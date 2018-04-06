#pragma once

#include <util/controls.h>
#include "module.h"
namespace DHE {

struct BicycleModule : Module {
  float roll_angle = 0.f;
  float pole_angle = 0.f;
  BicycleModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  }

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  float length(int knob, int cv, int av) {
    static auto length_range = Range{1e-3f, 1.f};
    return length_range.clamp(modulated(knob, cv, av));
  }

  float gain(int knob, int cv) {
    return gain_range().scale(modulated(knob, cv));
  }

  float speed() {
    auto modulated_speed = modulated(SPEED_KNOB, SPEED_CV, SPEED_CV_ATTENUVERTER);
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
    auto base_radius = length(BASE_RADIUS_KNOB, BASE_RADIUS_CV, BASE_CV_ATTENUVERTER);
    auto roller_radius = length(ROLL_RADIUS_KNOB, ROLL_RADIUS_CV, ROLL_CV_ATTENUVERTER);
    auto pole_length = length(POLE_LENGTH_KNOB, POLE_LENGTH_CV, POLE_CV_ATTENUVERTER);

    auto direction = param(ROLL_POSITION_SWITCH) > 0.5f ? 1.f : -1.f;
    auto roll_radius = std::abs(base_radius + direction*roller_radius);
    auto pole_angle_multiplier = roll_radius/roller_radius;

    roll_angle = increment_angle(roll_angle);
    pole_angle = increment_angle(pole_angle, pole_angle_multiplier);

    auto x = roll_radius*std::cos(roll_angle) - direction*pole_length*std::cos(pole_angle);
    auto y = roll_radius*std::sin(roll_angle) - pole_length*std::sin(pole_angle);

    auto roulette_radius = roll_radius + pole_length;
    auto scale = 5.f/roulette_radius;
    auto x_gain = gain(X_GAIN_KNOB, X_GAIN_CV);
    auto y_gain = gain(Y_GAIN_KNOB, Y_GAIN_CV);
    auto x_offset = param(X_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    auto y_offset = param(Y_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    outputs[X_OUT].value = x_gain*(x*scale + x_offset);
    outputs[Y_OUT].value = y_gain*(y*scale + y_offset);
  }
  enum ParameterIds {
    BASE_RADIUS_KNOB,
    BASE_CV_ATTENUVERTER,
    ROLL_RADIUS_KNOB,
    ROLL_CV_ATTENUVERTER,
    ROLL_POSITION_SWITCH,
    POLE_LENGTH_KNOB,
    POLE_CV_ATTENUVERTER,
    SPEED_KNOB,
    SPEED_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    BASE_RADIUS_CV,
    ROLL_RADIUS_CV,
    POLE_LENGTH_CV,
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
