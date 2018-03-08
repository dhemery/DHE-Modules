#pragma once

#include <engine.hpp>

#include "module.h"
#include "util/controls.h"
#include "util/d-flip-flop.h"
#include "util/ramp.h"
#include "util/range.h"
#include "util/track-and-hold.h"

namespace DHE {

struct StageModule : public Module {
  StageModule()
      : Module(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT),
        defer_gate{[this] { return defer_gate_in(); }},
        eoc_pulse{1e-3, [this] { return sample_time(); }},
        envelope{[this] { return duration_in(); }, [this] { return sample_time(); }},
        envelope_gate{[this] { return envelope_gate_in(); }},
        phase_0_voltage{[this] { return envelope_in(); }} {
    defer_gate.on_rising_edge([this] { on_defer_gate_rising(); });
    defer_gate.on_falling_edge([this] { on_defer_gate_falling(); });

    envelope_gate.on_rising_edge([this] { on_envelope_gate_rising(); });

    envelope.on_completion([this] { on_envelope_completion(); });
  }

  void begin_deferring() {
    envelope_gate.suspend_firing();
    envelope.stop();
    phase_0_voltage.track();
  }

  float defer_gate_in() const {
    return input(DEFER_IN);
  }

  float duration_in() const {
    return Duration::scaled(param(DURATION_KNOB), Duration::MEDIUM_RANGE);
  }

  float envelope_gate_in() const {
    return input(TRIGGER_IN);
  }

  float envelope_in() const {
    return input(ENVELOPE_IN);
  }

  float envelope_out(float phase_0_voltage, float phase) const {
    return scale(taper(phase), phase_0_voltage, level_in());
  }

  bool is_active() const {
    return defer_gate.is_high() || envelope.is_active();
  }

  float level_in() const {
    return Level::scaled(param(LEVEL_KNOB));
  }

  void on_defer_gate_falling() {
    stop_deferring();
  }

  void on_defer_gate_rising() {
    begin_deferring();
  }

  void on_envelope_completion() {
    eoc_pulse.start();
  }

  void on_envelope_gate_rising() {
    start_envelope();
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  void send_active_out(bool is_active) {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active);
  }

  void send_envelope_out(float phase_0_voltage, float phase) {
    outputs[ENVELOPE_OUT].value = envelope_out(phase_0_voltage, phase);
  }

  void send_eoc_out(bool is_pulsing) {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_pulsing);
  }

  void start_envelope() {
    phase_0_voltage.hold();
    envelope.start();
  }

  void step() override {
    defer_gate.step();
    envelope.step();
    envelope_gate.step();
    eoc_pulse.step();

    send_active_out(is_active());
    send_eoc_out(eoc_pulse.is_active());
    send_envelope_out(phase_0_voltage.value(), envelope.phase());
  }

  void stop_deferring() {
    phase_0_voltage.hold();
    envelope_gate.resume_firing();
  }

  float taper(float phase) const {
    return Taper::j(phase, param(CURVE_KNOB));
  }

  DFlipFlop defer_gate;
  Ramp eoc_pulse;
  Ramp envelope;
  DFlipFlop envelope_gate;
  TrackAndHold phase_0_voltage;

  enum ParameterIIds {
    DURATION_KNOB,
    LEVEL_KNOB,
    CURVE_KNOB,
    PARAMETER_COUNT
  };

  enum InputIds {
    ENVELOPE_IN,
    TRIGGER_IN,
    DEFER_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    ENVELOPE_OUT,
    EOC_OUT,
    ACTIVE_OUT,
    OUTPUT_COUNT
  };
};
}