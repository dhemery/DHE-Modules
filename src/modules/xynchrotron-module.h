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
            [this] { return curl()*zing(); }
        },
        spinner{
            [this] { return spinner_length; },
            [this] { return zing(); }
        } {}

  float curl() const {
    static constexpr auto node_max = 10.f;
    static constexpr auto curl_out_range = Range{0.f, node_max};
    static constexpr auto curl_in_range = Range{0.f, -node_max};
    static constexpr auto curl_all_range = Range{-node_max, node_max};
    auto curl_range_switch = param(CURL_RANGE_SWITCH);
    auto curl_range =
        curl_range_switch < 0.7f ? curl_in_range : curl_range_switch > 1.3f ? curl_out_range : curl_all_range;
    return 1.f - curl_range.scale(modulated(CURL_KNOB, CURL_CV, CURL_CV_ATTENUVERTER));
  }

  float wobble() const {
    static constexpr auto wobble_range = Range{0.f, 1.f};
    return 1.f - wobble_range.clamp(modulated(WOBBLE_KNOB, WOBBLE_CV, WOBBLE_CV_ATTENUVERTER));
  }

  float zing() const {
    auto rotation = modulated(ZING_KNOB, ZING_CV, ZING_CV_ATTENUVERTER);
    auto bipolar = BIPOLAR_PHASE_RANGE.scale(rotation);
    auto tapered = sigmoid(bipolar, 0.8f);
    return -10.f*tapered*rack::engineGetSampleTime();
  }

  float gain(int knob, int cv) const {
    return gain_range().scale(modulated(knob, cv));
  }

  static const Range &gain_range() {
    static constexpr auto gain_range = Range{0.f, 2.f};
    return gain_range;
  }

  void step() override {
    spinner_length = 5.f*wobble();
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
    CURL_KNOB,
    CURL_CV_ATTENUVERTER,
    CURL_RANGE_SWITCH,
    WOBBLE_KNOB,
    WOBBLE_CV_ATTENUVERTER,
    ZING_KNOB,
    ZING_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    CURL_CV,
    WOBBLE_CV,
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
