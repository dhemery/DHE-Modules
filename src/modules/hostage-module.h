#pragma once

#include <algorithm>

#include "module.h"
#include "stage-processor.h"
#include "util/controls.h"

namespace DHE {

struct HostageModule : Module, StageProcessor {
  HostageModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT}, StageProcessor{} {};

  float defer_in() const override {
    return input(DEFER_IN);
  };

  float duration_in() const override {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  float envelope_voltage(float held, float phase) const override {
    return held;
  }

  float gate_in() const override {
    return input(HOLD_IN);
  }

  void send_active_out(float f) override {
    outputs[ACTIVE_OUT].value = f;
  }

  void send_eoc_out(float f) override {
    outputs[EOC_OUT].value = f;
  }

  void send_stage_out(float f) override {
    outputs[STAGE_OUT].value = f;
  }

  void step() override {
    StageProcessor::step();
  }

  float stage_in() const override {
    return input(STAGE_IN);
  }

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_SWITCH,
    MODE_SWITCH,
    PARAMETER_COUNT
  };

  enum InputIds {
    DEFER_IN,
    DURATION_CV,
    HOLD_IN,
    STAGE_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    ACTIVE_OUT,
    EOC_OUT,
    STAGE_OUT,
    OUTPUT_COUNT
  };
};

}
