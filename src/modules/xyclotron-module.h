#pragma once

#include <utility>
#include <util/controls.h>
#include <util/wheel.h>
#include "module.h"

namespace DHE {

struct XyclotronModule : Module {
  Wheel primary_wheel;
  Wheel secondary_wheel;

  XyclotronModule()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
        primary_wheel{
            [this] { return length(PRIMARY_WHEEL_RADIUS_KNOB, PRIMARY_WHEEL_RADIUS_CV, PRIMARY_WHEEL_RADIUS_CV_ATTENUVERTER); },
            [this] { return speed(PRIMARY_WHEEL_SPEED_KNOB, PRIMARY_WHEEL_SPEED_CV, PRIMARY_WHEEL_SPEED_CV_ATTENUVERTER); }
        },
        secondary_wheel{
            [this] { return length(SECONDARY_WHEEL_RADIUS_KNOB, SECONDARY_WHEEL_RADIUS_CV, SECONDARY_WHEEL_RADIUS_CV_ATTENUVERTER); },
            [this] { return speed(SECONDARY_WHEEL_SPEED_KNOB, SECONDARY_WHEEL_SPEED_CV, SECONDARY_WHEEL_SPEED_CV_ATTENUVERTER); }
        } {}

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  float length(int knob, int cv, int av) const {
    return UNIPOLAR_PHASE_RANGE.scale(modulated(knob, cv, av));
  }

  float gain(int knob, int cv) const {
    return gain_range().scale(modulated(knob, cv));
  }

  float speed(int knob, int cv, int av) const {
    auto rotation = modulated(knob, cv, av);
    auto bipolar = BIPOLAR_PHASE_RANGE.scale(rotation);
    auto tapered = sigmoid(bipolar, 0.8f);
    return -10.f*tapered*rack::engineGetSampleTime();
  }

  void step() override {
    primary_wheel.step();
    secondary_wheel.step();

    auto x = primary_wheel.x() + secondary_wheel.x();
    auto y = primary_wheel.y() + secondary_wheel.y();

    auto roulette_radius = primary_wheel.radius() + secondary_wheel.radius();
    auto roulette_scale = 5.f/roulette_radius;
    auto x_gain = gain(X_GAIN_KNOB, X_GAIN_CV);
    auto y_gain = gain(Y_GAIN_KNOB, Y_GAIN_CV);
    auto x_offset = param(X_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    auto y_offset = param(Y_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;

    outputs[X_OUT].value = x_gain*(x*roulette_scale + x_offset);
    outputs[Y_OUT].value = y_gain*(y*roulette_scale + y_offset);
  }

  enum ParameterIds {
    PRIMARY_WHEEL_RADIUS_KNOB,
    PRIMARY_WHEEL_RADIUS_CV_ATTENUVERTER,
    PRIMARY_WHEEL_SPEED_KNOB,
    PRIMARY_WHEEL_SPEED_CV_ATTENUVERTER,
    SECONDARY_WHEEL_RADIUS_KNOB,
    SECONDARY_WHEEL_RADIUS_CV_ATTENUVERTER,
    SECONDARY_WHEEL_SPEED_KNOB,
    SECONDARY_WHEEL_SPEED_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    PRIMARY_WHEEL_RADIUS_CV,
    PRIMARY_WHEEL_SPEED_CV,
    SECONDARY_WHEEL_RADIUS_CV,
    SECONDARY_WHEEL_SPEED_CV,
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
