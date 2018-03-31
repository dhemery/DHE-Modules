#pragma once

#include "module.h"
namespace DHE {

struct CubicModule : Module {
  CubicModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  void step() override {
    outputs[OUT].value = BIPOLAR_SIGNAL_RANGE.scale(param(CONSTANT));
  }

  enum ParameterIds {
    CONSTANT,
    PARAMETER_COUNT
  };
  enum InputIds {
    INPUT_COUNT
  };
  enum OutputIds {
    OUT,
    OUTPUT_COUNT
  };
};

}