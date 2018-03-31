#pragma once

#include "module.h"
namespace DHE {

struct CubicModule : Module {
  CubicModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  float coefficient(float rotation) {
    static Range coefficient_knob_range = Range{-2.0f, 2.0f};
    return coefficient_knob_range.scale(rotation);
  }

  void step() override {
    auto x = input(X);
    auto offset = BIPOLAR_SIGNAL_RANGE.scale(param(OFFSET_KNOB));
    auto x_scaled = x * coefficient(param(SCALE_KNOB));
    outputs[Y].value = x_scaled + offset;
  }

  enum ParameterIds {
    OFFSET_KNOB,
    SCALE_KNOB,
    SQUARED,
    CUBED,
    PARAMETER_COUNT
  };
  enum InputIds {
    X,
    INPUT_COUNT
  };
  enum OutputIds {
    Y,
    OUTPUT_COUNT
  };
};

}