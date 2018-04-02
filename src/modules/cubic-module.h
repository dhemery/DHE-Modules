#pragma once

#include "module.h"
namespace DHE {

struct CubicModule : Module {
  CubicModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  void step() override {
    auto input_gain = param(INPUT_GAIN_KNOB);
    auto output_gain = param(OUTPUT_GAIN_KNOB);

    auto x = input_gain*input(X)/5.f;
    auto x2 = x*x;
    auto x3 = x2*x;

    auto a = param(A_KNOB);
    auto b = param(B_KNOB);
    auto c = param(C_KNOB);
    auto d = param(D_KNOB);

    auto y = output_gain*(a*x3 + b*x2 + c*x + d);

    outputs[Y].value = 5.f*y;
  }

  enum ParameterIds {
    A_KNOB,
    B_KNOB,
    C_KNOB,
    D_KNOB,
    INPUT_GAIN_KNOB,
    OUTPUT_GAIN_KNOB,
    PARAMETER_COUNT
  };
  enum InputIds {
    X,
    A_CV,
    B_CV,
    C_CV,
    D_CV,
    INPUT_GAIN_CV,
    OUTPUT_GAIN_CV,
    INPUT_COUNT
  };
  enum OutputIds {
    Y,
    OUTPUT_COUNT
  };
};

}