#pragma once

#include <util/d-flip-flop.h>
#include <util/ramp.h>
#include <util/track-and-hold.h>
#include "util/controls.h"
#include "module.h"

namespace DHE {
struct BoosterStageModule : Module {
  BoosterStageModule()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
        defer_gate{[this] { return defer_gate_in(); }},
        eoc_pulse{1e-3, [this] { return sample_time(); }},
        envelope{[this] { return duration_in(); }, [this] { return sample_time(); }},
        envelope_gate{[this] { return envelope_gate_in(); }},
        phase_0_voltage{[this] { return envelope_in(); }} {
    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { stop_deferring(); });

    envelope_gate.on_rising_edge([this] { start_envelope(); });

    envelope.on_completion([this] { eoc_pulse.start(); });
  }

  void step() {
    defer_gate.step();
    envelope.step();
    envelope_gate.step();
    eoc_pulse.step();

    send_active_out();
    send_eoc_out();
    send_envelope_out();
  }

  void begin_deferring() {
    envelope_gate.suspend_firing();
    envelope.stop();
    phase_0_voltage.track();
  }

  float defer_gate_in() const {
    return std::max(input(DEFER_IN), gate_button(DEFER_BUTTON));
  }

  float duration_in() const {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  float envelope_gate_in() const {
    return std::max(input(TRIGGER_IN), gate_button(TRIGGER_BUTTON));
  }

  float envelope_in() const {
    return input(ENVELOPE_IN);
  }

  float envelope_out(float phase_0_voltage, float phase) const {
    return scale(taper(phase), phase_0_voltage, level_in());
  }

  bool is_s_taper() const {
    return param(SHAPE_SWITCH) > 0.5;
  }

  float level_in() const {
    const auto &range = Level::range(param(LEVEL_SWITCH));
    auto rotation = modulated(LEVEL_KNOB, LEVEL_CV);
    return Level::scaled(rotation, range);
  }

  void send_active_out() {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(defer_gate.is_high() || envelope.is_active() || param(ACTIVE_BUTTON) > 0.5f);
  }

  void send_envelope_out() {
    outputs[ENVELOPE_OUT].value = envelope_out(phase_0_voltage.value(), envelope.phase());
  }

  void send_eoc_out() {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(eoc_pulse.is_active() || param(EOC_BUTTON) > 0.5f);
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  void start_envelope() {
    phase_0_voltage.hold();
    envelope.start();
  }

  void stop_deferring() {
    phase_0_voltage.hold();
    envelope_gate.resume_firing();
  }

  float taper(float phase) const {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    return is_s_taper() ? Taper::s(phase, rotation) : Taper::j(phase, rotation);
  }

  DFlipFlop defer_gate;
  Ramp eoc_pulse;
  Ramp envelope;
  DFlipFlop envelope_gate;
  TrackAndHold phase_0_voltage;

  enum ParameterIds {
    ACTIVE_BUTTON,
    CURVE_KNOB,
    DEFER_BUTTON,
    DURATION_KNOB,
    DURATION_SWITCH,
    EOC_BUTTON,
    LEVEL_KNOB,
    LEVEL_SWITCH,
    SHAPE_SWITCH,
    TRIGGER_BUTTON,
    PARAMETER_COUNT
  };

  enum InputIds {
    CURVE_CV,
    DEFER_IN,
    DURATION_CV,
    LEVEL_CV,
    ENVELOPE_IN,
    TRIGGER_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    ACTIVE_OUT,
    EOC_OUT,
    ENVELOPE_OUT,
    OUTPUT_COUNT
  };
};
}
