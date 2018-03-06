#pragma once

#include <algorithm>

#include <engine.hpp>

#include "module.h"
#include "util/controls.h"

namespace DHE {

struct SnobModule : Module {
  SnobModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  bool is_s_taper() const {
    return param(SHAPE_SWITCH) > 0.5f;
  }

  void step() override {
  }

  float taper(float phase) const {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    return is_s_taper() ? Taper::s(phase, rotation) : Taper::j(phase, rotation);
  }

  enum ParameterIds {
    LEVEL_KNOB, LEVEL_SWITCH,
    CURVE_KNOB, SHAPE_SWITCH,
    PARAMETER_COUNT
  };

  enum InputIds {
    LEVEL_CV,
    CURVE_CV,
    INPUT_COUNT
  };
  enum OutputIds {
    SNOB_OUT,
    OUTPUT_COUNT
  };
};

}