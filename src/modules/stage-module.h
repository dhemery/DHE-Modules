#pragma once

#include <engine.hpp>

#include "module.h"
#include "util/controls.h"
#include "util/d-flip-flop.h"
#include "util/mode.h"
#include "util/ramp.h"
#include "util/range.h"

namespace DHE {

struct StageModule : public Module {
  Mode stage_mode{};
  Mode deferring_mode{};
  Mode *mode{&stage_mode};

  // TODO: Move this inside stage mode or an envelope class.
  float phase_0_voltage{0.f};

  DFlipFlop defer_gate = DFlipFlop{[this] { return defer_gate_in(); }};
  Ramp envelope = {[this] { return duration_in(); }, [this] { return sample_time(); }};
  DFlipFlop envelope_trigger = DFlipFlop{[this] { return envelope_gate_in(); }};
  Ramp eoc_pulse = {1e-3, [this] { return sample_time(); }};

  StageModule() : Module(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT) {
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

    enter_mode(mode);
  }

  void enter_mode(Mode *incoming_mode) {
    mode->exit();
    mode = incoming_mode;
    mode->enter();
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

  float envelope_voltage(float phase) const {
    return scale(taper(phase), phase_0_voltage, level_in());
  }

  float level_in() const {
    return Level::scaled(param(LEVEL_KNOB));
  }

  float sample_time() const {
    return rack::engineGetSampleTime();
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

  void step() override {
    defer_gate.step();
    mode->step();
    eoc_pulse.step();
  }

  float taper(float phase) const {
    return Taper::j(phase, param(CURVE_KNOB));
  }

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