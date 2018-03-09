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
  DFlipFlop sustain_mode;
  DFlipFlop *mode{nullptr};
  DFlipFlop mode_switch;
  TrackAndHold output_voltage;
  DFlipFlop duration_mode;

  HostageModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
                    defer_gate{[this] { return defer_gate_in(); }},
                    eoc_pulse{1e-3, [this] { return sample_time(); }},
                    envelope{[this] { return duration_in(); }, [this] { return sample_time(); }},
                    sustain_mode{[this] { return hold_in(); }},
                    mode_switch{[this] { return gate_mode_in(); }},
                    output_voltage{[this] { return envelope_in(); }},
                    duration_mode{[this] { return hold_in(); }} {
    mode_switch.on_rising_edge([this] { enter_sustain_mode(); });
    mode_switch.on_falling_edge([this] { enter_duration_mode(); });

    defer_gate.on_rising_edge([this] { begin_deferring(); });
    defer_gate.on_falling_edge([this] { end_deferring(); });

    duration_mode.on_rising_edge([this] { begin_envelope(); });
    envelope.on_completion([this] { end_cycle(); });

    sustain_mode.on_rising_edge([this] { begin_sustain(); });
    sustain_mode.on_no_change([this] { send_active(sustain_mode.is_high()); });
    sustain_mode.on_falling_edge([this] { end_cycle(); });

    mode = &sustain_mode;
    mode->resume_firing();
  }

  void step() {
    defer_gate.step();
    mode_switch.step();
    mode->step();
    envelope.step();
    eoc_pulse.step();

    send_eoc_out();
    send_envelope_out();
  }

  // DEFER mode
  float defer_gate_in() const {
    return input(DEFER_IN);
  };

  void begin_deferring() {
    mode->suspend_firing();
    envelope.stop();
    output_voltage.track();
    send_active(true);
  }

  void end_deferring() {
    output_voltage.hold();
    mode->resume_firing();
  }

  // Choose GATE/DUR mode for when not DEFERring
  float gate_mode_in() const {
    return param(GATE_MODE_SWITCH);
  }

  void enter_mode(DFlipFlop *new_mode) {
    mode->suspend_firing();
    mode = new_mode;
    mode->resume_firing();
    envelope.stop();
  }

  // SUSTAIN mode (active while gate is high)
  void enter_sustain_mode() {
    enter_mode(&sustain_mode);
  }

  void begin_sustain() {}

  // DURATION mode (active while ramp is running)
  void enter_duration_mode() {
    enter_mode(&duration_mode);
  }

  void begin_envelope() {
    send_active(true);
    envelope.start();
  }

  void end_cycle() {
    send_active(false);
    eoc_pulse.start();
  }

  float duration_in() const {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  float hold_in() const {
    return input(HOLD_IN);
  }

  float envelope_in() const {
    return input(ENVELOPE_IN);
  }

  void send_active(bool is_active) {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active);
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
    HOLD_IN,
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
