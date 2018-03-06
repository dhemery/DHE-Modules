#pragma once

#include <engine.hpp>

#include "module.h"
#include "util/controls.h"
#include "util/d-flip-flop.h"
#include "util/interval.h"
#include "util/ramp.h"
#include "util/sigmoid.h"
#include "util/track-and-hold-amplifier.h"

namespace DHE {

struct StageModule : public Module {
  enum ParamIds {
    DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB,
    PARAM_COUNT
  };

  enum InputIds {
    STAGE_IN, TRIG_IN, DEFER_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    STAGE_OUT, EOC_OUT, ACTIVE_OUT,
    OUTPUT_COUNT
  };

  StageModule() : StageModule(PARAM_COUNT, INPUT_COUNT, OUTPUT_COUNT) {}

  StageModule(int param_count, int input_count, int output_count) :
      Module{param_count, input_count, output_count},
      defer_gate{[this] { return defer_in(); }},
      end_of_cycle_pulse{1e-3, [this] { return sample_time(); }},
      envelope_ramp{[this] { return duration_in(); }, [this] { return sample_time(); }},
      envelope_trigger{[this] { return trigger_in(); }},
      stage_in{[this] { return this->input(STAGE_IN); }} {
    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { stop_deferring(); });

    envelope_trigger.on_rising_edge([this] { start_envelope(); });

    envelope_ramp.on_end_of_cycle([this] { end_of_cycle_pulse.start(); });
  }

  virtual float shape(float phase) const {
    return Shape::shape(phase, param(CURVE_KNOB));
  }

  float active_out() const {
    return UNIPOLAR_CV.scale(is_active());
  }

  virtual float defer_in() const {
    return input(DEFER_IN);
  }

  virtual float duration_in() const {
    return Duration::scaled(param(DURATION_KNOB), Duration::MEDIUM_RANGE);
  }

  float eoc_out() const {
    return UNIPOLAR_CV.scale(is_end_of_cycle());
  }

  virtual bool is_active() const {
    return defer_gate.is_high() || envelope_ramp.is_active();
  }

  virtual bool is_end_of_cycle() const {
    return end_of_cycle_pulse.is_active();
  }

  virtual float level_in() const {
    return Level::scaled(param(LEVEL_KNOB));
  }

  float stage_out() const {
    return defer_gate.is_high() ? stage_in.value() : envelope_voltage();
  }

  virtual float trigger_in() const {
    return input(TRIG_IN);
  }

  void step() override {
    defer_gate.step();
    envelope_ramp.step();
    envelope_trigger.step();
    end_of_cycle_pulse.step();

    outputs[ACTIVE_OUT].value = active_out();
    outputs[EOC_OUT].value = eoc_out();
    outputs[STAGE_OUT].value = stage_out();
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