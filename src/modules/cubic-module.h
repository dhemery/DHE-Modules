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

  float offset() {
    return BIPOLAR_SIGNAL_RANGE.scale(param(OFFSET_KNOB));
  }

  float scaled(float x) {
    return x * coefficient(param(SCALE_KNOB));
  }

  float squared(float x) {
    auto normalized = x / 5.f;
    auto squared = normalized * normalized;
    return 5.f * squared * coefficient(param(SQUARED_KNOB));
  }

  void step() override {
    auto x = input(X);
    outputs[Y].value = squared(x) + scaled(x) + offset();
  }

  enum ParameterIds {
    OFFSET_KNOB,
    SCALE_KNOB,
    SQUARED_KNOB,
    CUBED_KNOB,
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