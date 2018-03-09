#pragma once

#include <util/d-flip-flop.h>
#include <util/ramp.h>
#include <util/mode.h>
#include "util/controls.h"
#include "module.h"

namespace DHE {
struct BoosterStageModule : Module {
  Mode stage_mode{};
  Mode deferring_mode{};
  Mode *mode{&stage_mode};

  // TODO: Move this inside stage mode or an envelope class.
  float phase_0_voltage{0.f};

  DFlipFlop defer_gate = DFlipFlop{[this] { return defer_gate_in(); }};
  Ramp envelope = {[this] { return duration_in(); }, [this] { return sample_time(); }};
  Ramp eoc_pulse = {1e-3, [this] { return sample_time(); }};
  DFlipFlop envelope_trigger = DFlipFlop{[this] { return envelope_gate_in(); }};

  BoosterStageModule()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
    defer_gate.on_rising_edge([this] {
      enter_mode(&deferring_mode);
    });
    defer_gate.on_falling_edge([this] {
      enter_mode(&stage_mode);
    });

    deferring_mode.on_entry([this] {
      send_active(true);
    });
    deferring_mode.on_step([this] {
      send_envelope(envelope_in());
    });

    stage_mode.on_entry([this] {
      send_active(false);
      phase_0_voltage = envelope_in();
      envelope_trigger.resume_firing();
    });
    stage_mode.on_step([this] {
      envelope_trigger.step();
      envelope.step();
      send_envelope(envelope_voltage(envelope.phase())); // TODO: Move to envelope.on_step()
    });
    stage_mode.on_exit([this] {
      envelope_trigger.suspend_firing();
      envelope.stop();
    });

    envelope_trigger.on_rising_edge([this] {
      phase_0_voltage = envelope_in();
      envelope.start();
    });

    envelope.on_start([this] {
      send_active(true);
    });
    envelope.on_completion([this] {
      send_active(false);
      eoc_pulse.start();
    });

    eoc_pulse.on_start([this]{
      send_eoc(true);
    });
    eoc_pulse.on_completion([this] {
      send_eoc(false);
    });

    enter_mode(mode);  }

  void step() {
    defer_gate.step();
    mode->step();
    eoc_pulse.step();
  }

  void enter_mode(Mode *incoming_mode) {
    mode->exit();
    mode = incoming_mode;
    mode->enter();
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

  float envelope_voltage(float phase) const {
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

  void send_active(bool is_active) {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active || param(ACTIVE_BUTTON) > 0.5f);
  }

  void send_envelope(float voltage) {
    outputs[ENVELOPE_OUT].value = voltage;
  }

  void send_eoc(bool is_pulsing) {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_pulsing || param(EOC_BUTTON) > 0.5f);
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
  }

  float taper(float phase) const {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    return is_s_taper() ? Taper::s(phase, rotation) : Taper::j(phase, rotation);
  }

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
