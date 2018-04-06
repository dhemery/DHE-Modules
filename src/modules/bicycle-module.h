#pragma once

#include <util/controls.h>
#include "module.h"
namespace DHE {

struct BicycleModule : Module {
  float roller_angle = 0.f;
  float arm_angle = 0.f;
  BicycleModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  float length(int knob, int cv) {
    static auto length_range = Range{1e-3f, 1.f};
    return length_range.clamp(modulated(knob, cv));
  }

  float gain(int knob, int cv) {
    return gain_range().scale(modulated(knob, cv));
  }

  float speed() {
    auto modulated_speed = modulated(SPEED_KNOB, SPEED_CV);
    auto inverted_speed = 1.f - modulated_speed;
    auto clamped_speed = DHE::UNIPOLAR_PHASE_RANGE.clamp(inverted_speed);
    return Duration::scaled(clamped_speed, Duration::MEDIUM_RANGE);
  }

  float increment_angle(float angle, float multiplier = 1.f) {
    static auto two_pi = 2.f*std::acos(-1.f);
    angle += multiplier*two_pi*rack::engineGetSampleTime()/speed();
    if (angle > two_pi) angle -= two_pi;
    return angle;
  }

  void step() override {
    auto ring_radius = length(RING_RADIUS_KNOB, RING_RADIUS_CV);
    auto roller_radius = length(ROLLER_RADIUS_KNOB, ROLLER_RADIUS_CV);
    auto arm_length = length(ARM_LENGTH_KNOB, ARM_LENGTH_CV);

    auto roller_distance = std::abs(ring_radius - roller_radius);
    auto arm_angle_multiplier = roller_distance/roller_radius;

    roller_angle = increment_angle(roller_angle);
    arm_angle = increment_angle(arm_angle, arm_angle_multiplier);

    auto x = roller_distance*std::cos(roller_angle) + arm_length*std::cos(arm_angle);
    auto y = roller_distance*std::sin(roller_angle) - arm_length*std::sin(arm_angle);

    auto reach = roller_distance + arm_length;
    auto scale = 5.f/reach;
    auto x_gain = gain(X_GAIN_KNOB, X_GAIN_CV);
    auto y_gain = gain(Y_GAIN_KNOB, Y_GAIN_CV);
    auto x_offset = param(X_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    auto y_offset = param(Y_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    outputs[X_OUT].value = x_gain*(x*scale + x_offset);
    outputs[Y_OUT].value = y_gain*(y*scale + y_offset);
  }

  enum ParameterIds {
    RING_RADIUS_KNOB,
    ROLLER_RADIUS_KNOB,
    ARM_LENGTH_KNOB,
    SPEED_KNOB,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    RING_RADIUS_CV,
    ROLLER_RADIUS_CV,
    ARM_LENGTH_CV,
    SPEED_CV,
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
