#pragma once

#include <algorithm>

#include "module.h"
#include "util/controls.h"
#include "util/range.h"
#include "util/d-flip-flop.h"
#include "util/ramp.h"
#include "util/track-and-hold.h"

namespace DHE {

struct HostageModule : Module {
  HostageModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
                    defer_gate{[this] { return defer_in(); }},
                    end_of_cycle_pulse{1e-3, [this] { return sample_time(); }},
                    phase{[this] { return duration_in(); }, [this] { return sample_time(); }},
                    trigger{[this] { return trigger_in(); }},
                    tracker{[this] { return stage_in(); }} {
    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { stop_deferring(); });

    trigger.on_rising_edge([this] { start_envelope(); });

    phase.on_end_of_cycle([this] { end_of_cycle_pulse.start(); });
  }

  float defer_in() const {
    return input(DEFER_IN);
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  float duration_in() const {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  float trigger_in() const {
    return input(TRIG_IN);
  }

  float stage_in() const {
    return input(STAGE_IN);
  }

  void begin_deferring() {
    trigger.suspend_firing();
    phase.stop();
    tracker.track();
  }

  void stop_deferring() {
    tracker.hold();
    trigger.resume_firing();
  }

  void start_envelope() {
    tracker.hold();
    phase.start();
  }

  float stage_out() const {
    return tracker.value();
  }

  void step() override {
    defer_gate.step();
    phase.step();
    trigger.step();
    end_of_cycle_pulse.step();

    send_active_out(active_out());
    send_eoc_out(eoc_out());
    send_stage_out(stage_out());
  }

  bool is_end_of_cycle() const {
    return end_of_cycle_pulse.is_active();
  }

  bool is_active() const {
    return defer_gate.is_high() || phase.is_active();
  }

  float eoc_out() const {
    return UNIPOLAR_SIGNAL_RANGE.scale(is_end_of_cycle());
  }

  float active_out() const {
    return UNIPOLAR_SIGNAL_RANGE.scale(is_active());
  }

  void send_active_out(float f) {
    outputs[ACTIVE_OUT].value = f;
  }

  void send_eoc_out(float f) {
    outputs[EOC_OUT].value = f;
  }

  void send_stage_out(float f) {
    outputs[STAGE_OUT].value = f;
  }

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    DEFER_IN,
    DURATION_CV,
    STAGE_IN,
    TRIG_IN,
    INPUT_COUNT
  };
  enum OutputIds {
    ACTIVE_OUT,
    EOC_OUT,
    STAGE_OUT,
    OUTPUT_COUNT
  };

  DFlipFlop defer_gate;
  Ramp end_of_cycle_pulse;
  Ramp phase;
  DFlipFlop trigger;
  TrackAndHold tracker;
};

}