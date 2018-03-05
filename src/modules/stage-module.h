#pragma once

#include <engine.hpp>

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

  enum LightIds {
    NUM_LIGHTS
  };

  StageModule() : Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
                  defer_gate{[this] { return defer_input(); }},
                  end_of_cycle_pulse{1e-3, [this] { return sample_time(); }},
                  envelope_ramp{[this] { return duration(); }, [this] { return sample_time(); }},
                  envelope_trigger{[this] { return trigger_input(); }},
                  stage_input_tracker{[this] { return stage_input(); }} {
    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { stop_deferring(); });

    envelope_trigger.on_rising_edge([this] { start_envelope(); });

    envelope_ramp.on_end_of_cycle([this] { end_of_cycle_pulse.start(); });
  }

  DFlipFlop defer_gate;
  Ramp end_of_cycle_pulse;
  Ramp envelope_ramp;
  DFlipFlop envelope_trigger;
  TrackAndHoldAmplifier stage_input_tracker;

  void step() override {
    defer_gate.step();
    envelope_ramp.step();
    envelope_trigger.step();
    end_of_cycle_pulse.step();

    send_active(is_active());
    send_eoc(end_of_cycle_pulse.is_active());
    send_stage(defer_gate.is_high() ? stage_input_tracker.value() : envelope_voltage());

  }

  float sample_time() {
    return rack::engineGetSampleTime();
  }

  float input(int index) const { return inputs[index].value; }

  float param(int index) const { return params[index].value; }

  void send(int index, float f) { outputs[index].value = f; }

  float duration() const {
    static constexpr auto DURATION_KNOB_CURVATURE = 0.8f; // Yields ~1/10 of max at center position
    auto duration_amount = sigmoid(duration_proportion(), DURATION_KNOB_CURVATURE);
    return duration_range().scale(duration_amount);
  }

  Interval duration_range() const {
    static constexpr auto default_duration_range = Interval{1e-2f, 10.f};
    return default_duration_range;
  }

  float level() const {
    return UNIPOLAR_CV.scale(level_proportion());
  }

  float curvature() const {
    static constexpr auto curve_knob_curvature = -0.65f;
    return sigmoid(BIPOLAR_NORMAL.scale(curve_proportion()), curve_knob_curvature);
  }

  float curve_proportion() const {
    return param(CURVE_KNOB);
  }

  float defer_input() const {
    return input(DEFER_IN);
  }

  float duration_proportion() const {
    return param(DURATION_KNOB);
  }

  float level_proportion() const {
    return param(LEVEL_KNOB);
  }

  float stage_input() const {
    return input(STAGE_IN);
  }

  float trigger_input() const {
    return input(TRIG_IN);
  }

  void send_active(bool active) {
    send(ACTIVE_OUT, UNIPOLAR_CV.scale(active));
  }

  void send_eoc(bool eoc) {
    send(EOC_OUT, UNIPOLAR_CV.scale(eoc));
  }

  void send_stage(float f) {
    send(STAGE_OUT, f);
  }

private:
  void begin_deferring() {
    envelope_trigger.suspend_firing();
    envelope_ramp.stop();
    stage_input_tracker.track();
  }

  void stop_deferring() {
    stage_input_tracker.hold();
    envelope_trigger.resume_firing();
  }

  void start_envelope() {
    stage_input_tracker.hold();
    envelope_ramp.start();
  }

  float shape(float phase) const {
    return sigmoid(phase, curvature());
  }

  float envelope_voltage() const {
    auto range = Interval{stage_input_tracker.value(), level()};
    return range.scale(shape(envelope_ramp.phase()));
  }

  bool is_active() const { return defer_gate.is_high() || envelope_ramp.is_active(); }
};
}