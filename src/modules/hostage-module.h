#pragma once

#include <algorithm>
#include <util/mode.h>

#include "module.h"
#include "util/controls.h"
#include "util/d-flip-flop.h"
#include "util/ramp.h"

namespace DHE {

struct HostageModule : Module {
  Mode deferring_mode{};
  Mode timed_sustain_mode{};
  Mode gated_sustain_mode{};
  Mode *sustain_mode{&timed_sustain_mode};
  Mode *mode{sustain_mode};

  DFlipFlop defer_gate = DFlipFlop{[this] { return defer_gate_in(); }};
  Ramp eoc_pulse = {1e-3, [this] { return sample_time(); }};
  DFlipFlop mode_switch = DFlipFlop{[this] { return mode_switch_in(); }};
  DFlipFlop sustain_gate = DFlipFlop{[this] { return hold_gate_in(); }};
  DFlipFlop sustain_trigger = DFlipFlop{[this] { return hold_gate_in(); }};
  Ramp timer = {[this] { return duration_in(); }, [this] { return sample_time(); }};

  HostageModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    deferring_mode.on_entry([this] {
      send_active(true);
    });
    deferring_mode.on_step([this] {
      send_envelope(envelope_in());
    });

    timed_sustain_mode.on_entry([this] {
      sustain_trigger.resume_firing();
      send_active(false);
      send_envelope(envelope_in());
    });
    timed_sustain_mode.on_step([this] {
      sustain_trigger.step();
      timer.step();
    });
    timed_sustain_mode.on_exit([this] {
      sustain_trigger.suspend_firing();
      timer.stop();
    });

    sustain_trigger.on_rising_edge([this] {
      timer.start();
    });

    timer.on_start([this]{
      begin_sustaining();
    });
    timer.on_completion([this] {
      end_sustaining();
    });

    gated_sustain_mode.on_entry([this] {
      sustain_gate.step();
      sustain_gate.resume_firing();
      if(sustain_gate.is_high()) begin_sustaining();
      else end_sustaining();
    });
    gated_sustain_mode.on_step([this] {
      sustain_gate.step();
    });
    gated_sustain_mode.on_exit([this]{
      sustain_gate.suspend_firing();
    });

    sustain_gate.on_rising_edge([this] {
      begin_sustaining();
    });
    sustain_gate.on_falling_edge([this] {
      end_sustaining();
    });

    eoc_pulse.on_start([this] {
      send_eoc(true);
    });
    eoc_pulse.on_completion([this]{
      send_eoc(false);
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

    enter_mode(mode);
  }

  void end_sustaining() {
    send_active(false);
    eoc_pulse.start();
  }
  void begin_sustaining() { send_active(true); }

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
  }

  float defer_gate_in() const {
    return input(DEFER_IN);
  };

  float mode_switch_in() const {
    return param(GATE_MODE_SWITCH);
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

  void send_envelope(float voltage) {
    outputs[ENVELOPE_OUT].value = voltage;
  }

  void send_eoc(bool is_pulsing) {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_pulsing);
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
