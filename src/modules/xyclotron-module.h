#pragma once

#include <util/controls.h>
#include "module.h"
namespace DHE {

struct XyclotronModule : Module {
  float angle1 = 0.f;
  float angle2 = 0.f;
  XyclotronModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  float length(int knob, int cv, int av) {
    return UNIPOLAR_PHASE_RANGE.scale(modulated(knob, cv, av));
  }

  float gain(int knob, int cv) {
    return gain_range().scale(modulated(knob, cv));
  }

  float speed(int knob, int cv, int av) {
    auto rotation = modulated(knob, cv, av);
    auto bipolar = BIPOLAR_PHASE_RANGE.scale(rotation);
    auto tapered = sigmoid(bipolar, 0.8f);
    return -10.f*tapered;
  }

  float increment_angle(float angle, float speed) {
    static auto two_pi = 2.f*std::acos(-1.f);
    angle += speed*two_pi/rack::engineGetSampleRate();
    if (angle > two_pi) angle -= two_pi;
    if (angle < 0.f) angle += two_pi;
    return angle;
  }

  void step() override {
    auto radius1 = length(R1_KNOB, R1_CV, R1_CV_ATTENUVERTER);
    auto speed1 = speed(S1_KNOB, S1_CV, S1_CV_ATTENUVERTER);
    auto radius2 = length(R2_KNOB, R2_CV, R2_CV_ATTENUVERTER);
    auto speed2 = speed(S2_KNOB, S2_CV, S2_CV_ATTENUVERTER);

    angle1 = increment_angle(angle1, speed1);
    angle2 = increment_angle(angle2, speed2);

    auto x = radius1*std::cos(angle1) + radius2*std::cos(angle2);
    auto y = radius1*std::sin(angle1) + radius2*std::sin(angle2);

    auto roulette_radius = radius1 + radius2;
    auto roulette_scale = 5.f/roulette_radius;
    auto x_gain = gain(X_GAIN_KNOB, X_GAIN_CV);
    auto y_gain = gain(Y_GAIN_KNOB, Y_GAIN_CV);
    auto x_offset = param(X_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    auto y_offset = param(Y_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    outputs[X_OUT].value = x_gain*(x*roulette_scale + x_offset);
    outputs[Y_OUT].value = y_gain*(y*roulette_scale + y_offset);
  }
  enum ParameterIds {
    R1_KNOB,
    R1_CV_ATTENUVERTER,
    S1_KNOB,
    S1_CV_ATTENUVERTER,
    R2_KNOB,
    R2_CV_ATTENUVERTER,
    S2_KNOB,
    S2_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    R1_CV,
    S1_CV,
    R2_CV,
    S2_CV,
    X_GAIN_CV,
    Y_GAIN_CV,
    INPUT_COUNT
  };
  enum OutputIds {
    X_OUT,
    Y_OUT,
    OUTPUT_COUNT
  };
};

}
