#pragma once

#include <algorithm>

#include <engine.hpp>

#include "module.h"
#include "util/controls.h"
#include "util/interval.h"

namespace DHE {

struct SwaveModule : Module {
  enum ParamIds {
    CURVE_KNOB, SHAPE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    CURVE_CV, SWAVE_IN,
    INPUT_COUNT
  };
  enum OutputIds {
    SWAVE_OUT,
    OUTPUT_COUNT
  };

  SwaveModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  void step() override {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    const auto &shape_phase_range = Shape::range(param(SHAPE_SWITCH));
    auto audio = inputs[SWAVE_IN].value;
    auto normalized_audio = BIPOLAR_CV.normalize(audio);
    auto shaped_normalized_audio = Shape::shape(normalized_audio, rotation, shape_phase_range);
    outputs[SWAVE_OUT].value = BIPOLAR_CV.scale(shaped_normalized_audio);
  }
};

}