#pragma once

#include <algorithm>

#include "module.h"
#include "stage-processor.h"
#include "util/controls.h"

namespace DHE {

struct HostageModule : Module, StageProcessor {
  HostageModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT}, StageProcessor{} {};

  float defer_gate_in() const override {
    return input(DEFER_IN);
  };

  float duration_in() const override {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  float envelope_gate_in() const override {
    return input(HOLD_GATE_IN);
  }

  float envelope_in() const override {
    return input(ENVELOPE_IN);
  }

  bool is_gate_mode() const {
    return param(GATE_MODE_SWITCH) > 0.5f;
  }

  void on_envelope_gate_rising() override {
    if(is_gate_mode()) {
      phase_0_voltage.hold();
      envelope.stop();
    } else {
      envelope.start();
    }
  }

  void on_envelope_gate_falling() override {
    if(is_gate_mode()) {
      eoc_pulse.start();
    }
  }

  void send_active_out(bool is_active) override {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active);
  }

  void send_envelope_out(float phase_0_voltage, float ignored) override {
    outputs[ENVELOPE_OUT].value = phase_0_voltage;
  }

  void send_eoc_out(bool is_pulsing) override {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_pulsing);
  }

  void step() override {
    StageProcessor::step();
  }

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_SWITCH,
    GATE_MODE_SWITCH,
    PARAMETER_COUNT
  };

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
};

}
