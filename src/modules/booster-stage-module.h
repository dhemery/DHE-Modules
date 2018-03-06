#pragma once

#include <engine.hpp>
#include <util/controls.h>
#include "stage-module.h"

namespace DHE {
struct BoosterStageModule : StageModule {
  enum ParamIds {
    LEVEL_SWITCH = StageModule::NUM_PARAMS, SHAPE_SWITCH, DURATION_SWITCH,
    DEFER_BUTTON, TRIG_BUTTON, ACTIVE_BUTTON, EOC_BUTTON,
    NUM_PARAMS
  };
  enum InputIds {
    LEVEL_CV = StageModule::NUM_INPUTS, DURATION_CV, CURVE_CV,
    NUM_INPUTS
  };

  BoosterStageModule() : StageModule{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS} {}

  float curve_in() const override {
    return modulated(CURVE_KNOB, CURVE_CV);
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

  float shape(float phase) const override {
    const auto &range = param(SHAPE_SWITCH) > 0.5f ? BIPOLAR_NORMAL : NORMAL;
    auto scaled_phase = range.scale(phase);
    auto shaped_phase = StageModule::shape(scaled_phase);
    return range.normalize(shaped_phase);
  }

  float trigger_in() const override {
    return std::max(input(TRIG_IN), gate_button(TRIG_BUTTON));
  }

private:

  float gate_button(int index) const {
    return UNIPOLAR_CV.scale(param(index) > 0.5f);
  };

  float modulated(int param_index, int mod_index) const {
    return param(param_index) + input(mod_index)/10.f;
  }
};
}
