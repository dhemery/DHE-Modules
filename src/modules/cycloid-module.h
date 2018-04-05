#pragma once

#include "module.h"
namespace DHE {

struct CycloidModule : Module {
  float two_pi = 2.f*std::acos(-1.f);
  float phase = 0.f;
  CycloidModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  float length(int knob) {
    return std::round(20.f*param(knob));
  }

  void step() override {
    auto phase_increment = rack::engineGetSampleTime(); // 0 to 1 in 10s
    auto angle = two_pi*phase;
    auto fixed_radius = length(FIXED_RADIUS_KNOB);
    auto moving_radius = length(MOVING_RADIUS_KNOB);
    auto arm_length = length(ARM_LENGTH_KNOB);

    auto path_radius = std::abs(fixed_radius - moving_radius);
    auto arm_angle_multiplier = path_radius/moving_radius;

    auto reach = path_radius + std::abs(arm_length);
    auto scale = 5.f/reach;
    auto x = path_radius*std::cos(angle) + arm_length*std::cos(angle*arm_angle_multiplier);
    auto y = path_radius*std::sin(angle) - arm_length*std::sin(angle*arm_angle_multiplier);
    outputs[X_OUT].value = x*scale;
    outputs[Y_OUT].value = y*scale;

    auto max_phase = moving_radius;
    phase += phase_increment;
    if (phase > max_phase) phase -= max_phase;
  }

  enum ParameterIds {
    FIXED_RADIUS_KNOB,
    MOVING_RADIUS_KNOB,
    ARM_LENGTH_KNOB,
    PARAMETER_COUNT
  };
  enum InputIds {
    INPUT_COUNT
  };
  enum OutputIds {
    X_OUT,
    Y_OUT,
    OUTPUT_COUNT
  };
};

}
