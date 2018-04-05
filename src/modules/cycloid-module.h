#pragma once

#include "module.h"
namespace DHE {

struct CycloidModule : Module {
  float two_pi = 2.f*std::acos(-1.f);
  float arm_origin_angle = 0.f;
  float arm_rotation_angle = 0.f;
  CycloidModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  float length(int knob) {
    const auto length_range = Range{1e-3f, 1.f};
    return length_range.clamp(param(knob));
  }

  void step() override {
    auto angle_increment = two_pi*rack::engineGetSampleTime(); // 0 to 1 in 10s
    auto fixed_radius = length(FIXED_RADIUS_KNOB);
    auto moving_radius = length(MOVING_RADIUS_KNOB);
    auto arm_length = length(ARM_LENGTH_KNOB);

    auto path_radius = std::abs(fixed_radius - moving_radius);
    auto arm_rotation_angle_multiplier = path_radius/moving_radius;

    arm_origin_angle += angle_increment;
    if(arm_origin_angle > two_pi) arm_origin_angle -= two_pi;
    arm_rotation_angle += angle_increment*arm_rotation_angle_multiplier;
    if(arm_rotation_angle > two_pi) arm_rotation_angle -= two_pi;

    auto x = path_radius*std::cos(arm_origin_angle) + arm_length*std::cos(arm_rotation_angle);
    auto y = path_radius*std::sin(arm_origin_angle) - arm_length*std::sin(arm_rotation_angle);

    auto reach = path_radius + std::abs(arm_length);
    auto scale = 5.f/reach;
    outputs[X_OUT].value = x*scale;
    outputs[Y_OUT].value = y*scale;
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
