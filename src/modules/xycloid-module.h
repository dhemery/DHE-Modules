#pragma once

#include <util/controls.h>
#include <util/wheel.h>
#include "module.h"
namespace DHE {

struct XycloidModule : Module {
  Wheel rotor;
  Wheel spinner;
  float spinner_length{0.f};
  float rotor_length{0.f};

  XycloidModule()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
        rotor{
            [this] { return rotor_length; },
            [this] { return gear_ratio()*speed(); }
        },
        spinner{
            [this] { return spinner_length; },
            [this] { return speed(); }
        } {}

  float gear_ratio() const {
    static constexpr auto cusp_max = 16.f;
    static constexpr auto outward_cusps = Range{0.f, cusp_max};
    static constexpr auto inward_cusps = Range{0.f, -cusp_max};
    static constexpr auto inward_and_outward_cusps = Range{-cusp_max, cusp_max};
    static constexpr Range cusp_ranges[] = {inward_cusps, inward_and_outward_cusps, outward_cusps};
    auto cusp_type = static_cast<int>(param(CUSP_TYPE_SWITCH));
    auto cusp_range = cusp_ranges[cusp_type];
    float modulated_cusps = modulated(CUSP_KNOB, CUSP_CV, CUSP_CV_ATTENUVERTER);
    return 1.f - cusp_range.scale(modulated_cusps);
  }

  float depth() const {
    static constexpr auto wobble_range = Range{0.f, 1.f};
    return 1.f - wobble_range.clamp(modulated(DEPTH_KNOB, DEPTH_CV, DEPTH_CV_ATTENUVERTER));
  }

  float speed() const {
    auto rotation = modulated(SPEED_KNOB, SPEED_CV, SPEED_CV_ATTENUVERTER);
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
    spinner_length = 5.f*depth();
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
    CUSP_KNOB,
    CUSP_CV_ATTENUVERTER,
    CUSP_TYPE_SWITCH,
    DEPTH_KNOB,
    DEPTH_CV_ATTENUVERTER,
    SPEED_KNOB,
    SPEED_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    CUSP_CV,
    DEPTH_CV,
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
