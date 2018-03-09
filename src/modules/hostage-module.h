#pragma once

#include <algorithm>
#include <util/mode.h>

#include "module.h"
#include "util/controls.h"
#include "util/d-flip-flop.h"
#include "util/ramp.h"
#include "util/track-and-hold.h"

namespace DHE {

struct HostageModule : Module {
  Mode deferring_mode{};
  Mode timed_sustain_mode{};
  Mode gated_sustain_mode{};
  Mode *sustain_mode;
  Mode *mode;

  DFlipFlop defer_gate;
  Ramp eoc_pulse;
  DFlipFlop mode_switch;
  DFlipFlop sustain_gate;
  DFlipFlop sustain_trigger;
  Ramp timer;

  HostageModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
                    defer_gate{[this] { return defer_gate_in(); }},
                    eoc_pulse{1e-3, [this] { return sample_time(); }},
                    mode_switch{[this] { return mode_switch_in(); }},
                    sustain_gate{[this] { return hold_gate_in(); }},
                    sustain_trigger{[this] { return hold_gate_in(); }},
                    timer{[this] { return duration_in(); }, [this] { return sample_time(); }} {
    deferring_mode.on_entry([this] {
      send_active(true);
    });
    deferring_mode.on_step([this] {
      send_envelope_out(envelope_in());
    });

    timed_sustain_mode.on_entry([this] {
      send_active(false);
      send_envelope_out(envelope_in());
    });
    timed_sustain_mode.on_step([this] {
      sustain_trigger.step();
      timer.step();
    });
    timed_sustain_mode.on_exit([this] {
      timer.stop();
    });

    sustain_trigger.on_rising_edge([this] {
      begin_timed_sustain();
    });
    timer.on_completion([this] {
      end_cycle();
    });

    gated_sustain_mode.on_entry([this] {
      sustain_gate.step();
      send_active(sustain_gate.is_high());
    });
    gated_sustain_mode.on_step([this] {
      sustain_gate.step();
    });

    sustain_gate.on_rising_edge([this] {
      begin_gated_sustain();
    });
    sustain_gate.on_falling_edge([this] {
      end_cycle();
    });

    mode_switch.on_rising_edge([this] {
      set_sustain_mode(&gated_sustain_mode);
    });
    mode_switch.on_falling_edge([this] {
      set_sustain_mode(&timed_sustain_mode);
    });

    defer_gate.on_rising_edge([this] {
      enter_mode(&deferring_mode);
    });
    defer_gate.on_falling_edge([this] {
      enter_mode(sustain_mode);
    });

    sustain_mode = &gated_sustain_mode;
    mode = sustain_mode;
    mode->enter();
    mode_switch.step();
  }

  void set_sustain_mode(Mode *incoming_sustain_mode) {
    sustain_mode = incoming_sustain_mode;
    if (defer_gate.is_low()) {
      enter_mode(incoming_sustain_mode);
    }
  }

  void enter_mode(Mode *incoming_mode) {
    mode->exit();
    mode = incoming_mode;
    mode->enter();
  }

  void step() {
    defer_gate.step();
    mode_switch.step();
    mode->step();
    eoc_pulse.step();

    send_eoc_out();
  }

  float defer_gate_in() const {
    return input(DEFER_IN);
  };

  float mode_switch_in() const {
    return param(GATE_MODE_SWITCH);
  }

  void begin_gated_sustain() {
    send_active(true);
  }

  void begin_timed_sustain() {
    send_active(true);
    timer.start();
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

  float hold_gate_in() const {
    return input(HOLD_GATE_IN);
  }

  float envelope_in() const {
    return input(ENVELOPE_IN);
  }

  void send_active(bool is_active) {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active);
  }

  void send_envelope_out(float envelope_out) {
    outputs[ENVELOPE_OUT].value = envelope_out;
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
