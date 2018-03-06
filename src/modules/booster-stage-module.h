#pragma once

#include "stage-module.h"
#include "util/controls.h"

namespace DHE {
struct BoosterStageModule : StageModule {
  enum ParamIds {
    LEVEL_SWITCH = StageModule::PARAM_COUNT, SHAPE_SWITCH, DURATION_SWITCH,
    DEFER_BUTTON, TRIG_BUTTON, ACTIVE_BUTTON, EOC_BUTTON,
    PARAM_COUNT
  };

  enum InputIds {
    LEVEL_CV = StageModule::INPUT_COUNT, DURATION_CV, CURVE_CV,
    INPUT_COUNT
  };

  BoosterStageModule() : StageModule{PARAM_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  float shape(float phase) const override {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    const auto &range = Shape::range(param(SHAPE_SWITCH));
    return Shape::shape(phase, rotation, range);
  }

  float defer_in() const override {
    return std::max(input(DEFER_IN), gate_button(DEFER_BUTTON));
  }

  float duration_in() const override {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  bool is_active() const override {
    return param(ACTIVE_BUTTON) > 0.5f || StageModule::is_active();
  }

  bool is_end_of_cycle() const override {
    return param(EOC_BUTTON) > 0.5f || StageModule::is_end_of_cycle();
  }

  float level_in() const override {
    const auto &range = Level::range(param(LEVEL_SWITCH));
    auto rotation = modulated(LEVEL_KNOB, LEVEL_CV);
    return Level::scaled(rotation, range);
  }

  float trigger_in() const override {
    return std::max(input(TRIG_IN), gate_button(TRIG_BUTTON));
  }
};
}
