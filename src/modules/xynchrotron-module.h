#pragma once

#include <util/controls.h>
#include <util/wheel.h>
#include "module.h"
namespace DHE {

struct XynchrotronModule : Module {
  Wheel rotor;
  Wheel spinner;
  float spinner_length{0.f};
  float rotor_length{0.f};

  XynchrotronModule()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
        rotor{
            [this] { return rotor_length; },
            [this] { return gear_ratio()*spinner_speed(); }
        },
        spinner{
            [this] { return spinner_length; },
            [this] { return spinner_speed(); }
        } {}

  float gear_ratio() const {
    static constexpr auto gear_ratio_range = Range{-4.f, 6.f};
    return gear_ratio_range.scale(modulated(GEAR_RATIO_KNOB, GEAR_RATIO_CV, GEAR_RATIO_CV_ATTENUVERTER));
  }

  float spinner_ratio() const {
    static constexpr auto stick_ratio_range = Range{0.f, 1.f};
    return stick_ratio_range.clamp(modulated(SPINNER_RATIO_KNOB, SPINNER_RATIO_CV, SPINNER_RATIO_CV_ATTENUVERTER));
  }

  float gain(int knob, int cv) const {
    return gain_range().scale(modulated(knob, cv));
  }

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  float spinner_speed() const {
    auto rotation = modulated(SPEED_KNOB, SPEED_CV, SPEED_CV_ATTENUVERTER);
    auto bipolar = BIPOLAR_PHASE_RANGE.scale(rotation);
    auto tapered = sigmoid(bipolar, 0.8f);
    return -10.f*tapered*rack::engineGetSampleTime();
  }

  void step() override {
    spinner_length = 5.f*spinner_ratio();
    rotor_length = 5.f - spinner_length;

    rotor.step();
    spinner.step();

    auto x = rotor.x() + spinner.x();
    auto y = rotor.y() + spinner.y();

    auto x_gain = gain(X_GAIN_KNOB, X_GAIN_CV);
    auto y_gain = gain(Y_GAIN_KNOB, Y_GAIN_CV);
    auto x_offset = param(X_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    auto y_offset = param(Y_RANGE_SWITCH) > 0.5f ? 5.f : 0.f;
    outputs[X_OUT].value = x_gain*(x + x_offset);
    outputs[Y_OUT].value = y_gain*(y + y_offset);
  }
  enum ParameterIds {
    GEAR_RATIO_KNOB,
    GEAR_RATIO_CV_ATTENUVERTER,
    SPINNER_RATIO_KNOB,
    SPINNER_RATIO_CV_ATTENUVERTER,
    SPEED_KNOB,
    SPEED_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    GEAR_RATIO_CV,
    SPINNER_RATIO_CV,
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
