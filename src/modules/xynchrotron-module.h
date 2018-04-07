#pragma once

#include <util/controls.h>
#include <util/wheel.h>
#include "module.h"
namespace DHE {

struct XynchrotronModule : Module {
  Wheel primary_wheel;
  Wheel secondary_wheel;
  float primary_wheel_radius{0.f};
  float secondary_wheel_radius{0.f};
  float gear_ratio{0.f};

  XynchrotronModule()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
        primary_wheel{
            [this] { return primary_wheel_radius; },
            [this] { return speed(); }
        },
        secondary_wheel{
            [this] { return secondary_wheel_radius; },
            [this] { return gear_ratio*speed(); }
        } {}

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  float positive_length(int knob, int cv, int av) const {
    static auto length_range = Range{1e-2f, 1.f};
    return length_range.clamp(modulated(knob, cv, av));
  }

  float nonnegative_length(int knob, int cv, int av) const {
    return UNIPOLAR_PHASE_RANGE.clamp(modulated(knob, cv, av));
  }

  float gain(int knob, int cv) const {
    return gain_range().scale(modulated(knob, cv));
  }

  float speed() const {
    auto rotation = modulated(ZING_KNOB, ZING_CV, ZING_CV_ATTENUVERTER);
    auto bipolar = BIPOLAR_PHASE_RANGE.scale(rotation);
    auto tapered = sigmoid(bipolar, 0.8f);
    return -10.f*tapered*rack::engineGetSampleTime();
  }

  void step() override {
    secondary_wheel_radius = nonnegative_length(CURL_KNOB, CURL_CV, CURL_CV_ATTENUVERTER);
    auto base_radius = nonnegative_length(ROCK_KNOB, ROCK_CV, ROCK_CV_ATTENUVERTER);
    auto roller_radius = positive_length(ROLL_KNOB, ROLL_CV, ROLL_CV_ATTENUVERTER);
    primary_wheel_radius = base_radius + roller_radius;
    gear_ratio = primary_wheel_radius/roller_radius;

    primary_wheel.step();
    secondary_wheel.step();

    auto x = primary_wheel.x() + secondary_wheel.x();
    auto y = primary_wheel.y() - secondary_wheel.y();

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
    ROCK_KNOB,
    ROCK_CV_ATTENUVERTER,
    ROLL_KNOB,
    ROLL_CV_ATTENUVERTER,
    CURL_KNOB,
    CURL_CV_ATTENUVERTER,
    ZING_KNOB,
    ZING_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    ROCK_CV,
    ROLL_CV,
    CURL_CV,
    ZING_CV,
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
