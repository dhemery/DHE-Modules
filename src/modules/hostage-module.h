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

  float envelope_voltage(float start_voltage, float ignored) const override {
    return start_voltage;
  }

  void send_active_out(bool is_active) override {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active);
  }

  void send_eoc_out(bool is_pulsing) override {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_pulsing);
  }

  void send_envelope_out(float envelope_out) override {
    outputs[ENVELOPE_OUT].value = envelope_out;
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
