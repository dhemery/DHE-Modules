#pragma once

#include "module.h"
namespace DHE {


struct CubicModule : Module {
  CubicModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  static const Range &coefficient_range() {
    static constexpr auto coefficient_range = Range{-2.f, 2.f};
    return coefficient_range;
  }

  float gain(int knob) const {
    return gain_range().scale(param(knob));
  }

  float coefficient(int knob) const {
    return coefficient_range().scale(param(knob));
  }

  float a() const { return coefficient(A_KNOB); }
  float b() const { return coefficient(B_KNOB); }
  float c() const { return coefficient(C_KNOB); }
  float d() const { return coefficient(D_KNOB); }
  float input_gain() const { return gain(INPUT_GAIN_KNOB); }
  float output_gain() const { return gain(OUTPUT_GAIN_KNOB); }

  void step() override {
    auto x = input_gain()*input(X)/5.f;
    auto x2 = x*x;
    auto x3 = x2*x;

    auto y = output_gain()*(a()*x3 + b()*x2 + c()*x + d());

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