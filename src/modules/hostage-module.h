#pragma once

#include <algorithm>

#include "module.h"
#include "util/controls.h"
#include "util/d-flip-flop.h"
#include "util/ramp.h"
#include "util/track-and-hold.h"

namespace DHE {

struct HostageModule : Module {
  DFlipFlop defer_gate;
  Ramp eoc_pulse;
  Ramp envelope;
  DFlipFlop gate_mode;
  DFlipFlop *mode{nullptr};
  DFlipFlop mode_switch;
  TrackAndHold output_voltage;
  DFlipFlop trig_mode;

  HostageModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
                    defer_gate{[this] { return defer_gate_in(); }},
                    eoc_pulse{1e-3, [this] { return sample_time(); }},
                    envelope{[this] { return duration_in(); }, [this] { return sample_time(); }},
                    gate_mode{[this] { return envelope_gate_in(); }},
                    mode_switch{[this] { return gate_mode_in(); }},
                    output_voltage{[this] { return envelope_in(); }},
                    trig_mode{[this] { return envelope_gate_in(); }} {
    mode_switch.on_rising_edge([this] { enter_gate_mode(); });
    mode_switch.on_falling_edge([this] { enter_trig_mode(); });

    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { end_deferring(); });

    trig_mode.on_rising_edge([this] { begin_envelope(); });
    envelope.on_completion([this] { eoc_pulse.start(); });

    gate_mode.on_rising_edge([this] { begin_sustain(); });
    gate_mode.on_falling_edge([this] { eoc_pulse.start(); });

    mode = &gate_mode;
    mode->resume_firing();
  }

  void enter_mode(DFlipFlop *new_mode) {
    mode->suspend_firing();
    mode = new_mode;
    mode->resume_firing();
  }

  void enter_gate_mode() {
    enter_mode(&gate_mode);
  }

  void step() {
    defer_gate.step();
    mode_switch.step();
    mode->step();
    envelope.step();
    eoc_pulse.step();

    send_active_out();
    send_eoc_out();
    send_envelope_out();
  }

  void begin_deferring() {
    mode->suspend_firing();
    envelope.stop();
    output_voltage.track();
  }

  void end_deferring() {
    output_voltage.hold();
    mode->resume_firing();
  }

  void enter_trig_mode() {
    enter_mode(&trig_mode);
  }

  void begin_envelope() {
    output_voltage.hold();
    envelope.start();
  }

  void begin_sustain() {
    envelope.stop();
    output_voltage.hold();
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

  float gate_mode_in() const {
    return param(GATE_MODE_SWITCH);
  }

  bool is_active() const {
    return defer_gate.is_high() || envelope.is_active() ;
  }

  void send_active_out() {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active());
  }

  void send_envelope_out() {
    outputs[ENVELOPE_OUT].value = output_voltage.value();
  }

  void send_eoc_out() {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(eoc_pulse.is_active());
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

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
