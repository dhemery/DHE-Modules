#pragma once

#include <engine.hpp>

#include "util/controls.h"
#include "util/interval.h"
#include "util/sigmoid.h"
#include "util/d-flip-flop.h"
#include "util/ramp.h"
#include "util/track-and-hold-amplifier.h"

namespace DHE {
struct StageModule : rack::Module {
  enum ParamIds {
    DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB,
    NUM_PARAMS
  };
  enum InputIds {
    STAGE_IN, TRIG_IN, DEFER_IN,
    NUM_INPUTS
  };
  enum OutputIds {
    STAGE_OUT, EOC_OUT, ACTIVE_OUT,
    NUM_OUTPUTS
  };

  StageModule() : StageModule(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}

  StageModule(int numParams, int numInputs, int numOutputs) :
      rack::Module{numParams, numInputs, numOutputs},
      defer_gate{[this] { return defer_in(); }},
      end_of_cycle_pulse{1e-3, [this] { return sample_time(); }},
      envelope_ramp{[this] { return duration_in(); }, [this] { return sample_time(); }},
      envelope_trigger{[this] { return trigger_in() ; }},
      stage_in{[this] { return this->input(STAGE_IN); }} {
    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { stop_deferring(); });

    envelope_trigger.on_rising_edge([this] { start_envelope(); });

    envelope_ramp.on_end_of_cycle([this] { end_of_cycle_pulse.start(); });
  }

  virtual float curve_in() const {
    return param(CURVE_KNOB);
  }

  virtual float defer_in() const {
    return input(DEFER_IN);
  }

  virtual float duration_in() const {
    return Duration::scaled(param(DURATION_KNOB), Duration::MEDIUM_RANGE);
  }

  virtual bool is_active() const {
    return defer_gate.is_high() || envelope_ramp.is_active();
  }

  virtual bool is_end_of_cycle() const {
    return end_of_cycle_pulse.is_active();
  }

  virtual float level_in() const {
    return Level::scaled(param(LEVEL_KNOB), UNIPOLAR_CV);
  }

  virtual float shape(float phase) const {
    return sigmoid(phase, curvature());
  }

  virtual float trigger_in() const {
    return input(TRIG_IN);
  }

  float curvature() const {
    static constexpr auto curve_knob_curvature = -0.65f;
    return sigmoid(BIPOLAR_NORMAL.scale(curve_in()), curve_knob_curvature);
  }

  float input(int index) const {
    return inputs[index].value;
  }

  float param(int index) const {
    return params[index].value;
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  float &output(int index) {
    return outputs[index].value;
  }

  void send(int index, float f) { output(index) = f; }

  void send_active_out() {
    send(ACTIVE_OUT, UNIPOLAR_CV.scale(is_active()));
  }

  void send_eoc_out() {
    send(EOC_OUT, UNIPOLAR_CV.scale(is_end_of_cycle()));
  }

  void send_stage_out() {
    send(STAGE_OUT, defer_gate.is_high() ? stage_in.value() : envelope_voltage());
  }

  void step() override {
    defer_gate.step();
    envelope_ramp.step();
    envelope_trigger.step();
    end_of_cycle_pulse.step();

    send_active_out();
    send_eoc_out();
    send_stage_out();
  }

private:

  void begin_deferring() {
    envelope_trigger.suspend_firing();
    envelope_ramp.stop();
    stage_in.track();
  }

  float envelope_voltage() const {
    auto range = Interval{stage_in.value(), level_in()};
    return range.scale(shape(envelope_ramp.phase()));
  }

  void stop_deferring() {
    stage_in.hold();
    envelope_trigger.resume_firing();
  }

  void start_envelope() {
    stage_in.hold();
    envelope_ramp.start();
  }

  DFlipFlop defer_gate;
  Ramp end_of_cycle_pulse;
  Ramp envelope_ramp;
  DFlipFlop envelope_trigger;
  TrackAndHoldAmplifier stage_in;
};
}