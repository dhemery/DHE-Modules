#pragma once

#include <algorithm>

#include "module.h"
#include "stage-processor.h"
#include "util/controls.h"

namespace DHE {

struct HostageModule : Module {
  HostageModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
                    defer_gate{[this] { return defer_gate_in(); }},
                    eoc_pulse{1e-3, [this] { return sample_time(); }},
                    envelope{[this] { return duration_in(); }, [this] { return sample_time(); }},
                    envelope_gate{[this] { return envelope_gate_in(); }},
                    phase_0_voltage{[this] { return envelope_in(); }} {
    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { end_deferring(); });

    envelope_gate.on_rising_edge([this] { on_envelope_gate_rising(); });
    envelope_gate.on_falling_edge([this] { on_envelope_gate_falling(); });
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

  float defer_gate_in() const {
    return input(DEFER_IN);
  };

  float duration_in() const {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  float envelope_gate_in() const {
    return input(HOLD_GATE_IN);
  }

  float envelope_in() const {
    return input(ENVELOPE_IN);
  }

  bool is_in_gate_mode() const {
    return param(GATE_MODE_SWITCH) > 0.5f;
  }

  void on_envelope_gate_rising() {
    if (is_in_gate_mode()) {
      begin_sustain();
    } else {
      start_envelope();
    }
  }

  void on_envelope_gate_falling() {
    if (is_in_gate_mode()) {
      end_sustain();
    }
  }

  void send_active_out() {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active());
  }

  void send_envelope_out() {
    outputs[ENVELOPE_OUT].value = phase_0_voltage.value();
  }

  void send_eoc_out() {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(eoc_pulse.is_active());
  }

  void begin_sustain() {
    envelope.stop();
    phase_0_voltage.hold();
    is_sustaining = true;
  }

  void end_sustain() {
    is_sustaining = false;
    eoc_pulse.start();
  }

  bool is_active() const {
    return defer_gate.is_high() || envelope.is_active() || is_sustaining;
  }

  void begin_deferring() {
    envelope_gate.suspend_firing();
    envelope.stop();
    phase_0_voltage.track();
  }

  void end_deferring() {
    phase_0_voltage.hold();
    envelope_gate.resume_firing();
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  void start_envelope() {
    phase_0_voltage.hold();
    envelope.start();
  }

  DFlipFlop defer_gate;
  Ramp eoc_pulse;
  Ramp envelope;
  DFlipFlop envelope_gate;
  bool is_sustaining{false};
  TrackAndHold phase_0_voltage;

  enum InputIds {
    DEFER_IN,
    DURATION_CV,
    ENVELOPE_IN,
    HOLD_GATE_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    ACTIVE_OUT,
    ENVELOPE_OUT,
    EOC_OUT,
    OUTPUT_COUNT
  };

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_SWITCH,
    GATE_MODE_SWITCH,
    PARAMETER_COUNT
  };
};

}
