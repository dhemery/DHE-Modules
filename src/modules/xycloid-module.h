#pragma once

#include <util/controls.h>
#include "module.h"
namespace DHE {

struct Pole {
  const float two_pi = 2.f*std::acos(-1.f);
  float phase{0.f};
  float offset{0.f};

  void advance(float increment, float offset = 0.f) {
    this->offset = offset;
    phase += increment;
    phase -= std::trunc(phase); // Reduce phase to (-1, 1) to avoid eventual overflow
  }

  float x() const {
    return std::cos(two_pi*(phase+offset));
  }

  float y() const {
    return std::sin(two_pi*(phase+offset));
  }
};

struct XycloidModule : Module {
  Pole wobbler;
  Pole throbber;

  XycloidModule() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

  float wobble_ratio() const {
    static constexpr auto wobble_max = 16.f;
    static constexpr auto wobble_inward = Range{0.f, wobble_max};
    static constexpr auto wobble_outward = Range{0.f, -wobble_max};
    static constexpr auto bidirectional = Range{-wobble_max, wobble_max};
    static constexpr Range wobble_ratio_ranges[] = {wobble_inward, bidirectional, wobble_outward};
    auto wobble_type = static_cast<int>(param(WOBBLE_TYPE_SWITCH));
    auto quantize_wobble_ratio = param(QUANTIZE_WOBBLE_RATIO_SWITCH) < 1.f;
    auto wobble_range = wobble_ratio_ranges[wobble_type];
    auto wobble_ratio_rotation = modulated(WOBBLE_RATIO_KNOB, GEAR_RATIO_CV, WOBBLE_RATIO_CV_ATTENUVERTER);
    auto wobble_ratio = wobble_range.scale(wobble_ratio_rotation);
    if (quantize_wobble_ratio) wobble_ratio = std::round(wobble_ratio);
    return wobble_ratio;
  }

  float wobble_depth() const {
    static constexpr auto wobble_depth_range = Range{0.f, 1.f};
    return wobble_depth_range.clamp(modulated(WOBBLE_DEPTH_KNOB, DEPTH_CV, WOBBLE_DEPTH_CV_ATTENUVERTER));
  }

  float throb_speed(int knob, int cv, int attenuator) const {
    auto rotation = modulated(knob, cv, attenuator);
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
    auto wobble_ratio = this->wobble_ratio();
    auto wobble_phase_offset = param(WOBBLE_PHASE_KNOB)-0.5f;
    if (wobble_ratio < 0.f) wobble_phase_offset*=-1.f;

    auto throb_speed = this->throb_speed(THROB_SPEED_KNOB, SPEED_CV, THROB_SPEED_CV_ATTENUVERTER);
    auto wobble_speed = wobble_ratio*throb_speed;
    auto wobble_depth = this->wobble_depth();
    auto throb_depth = 1.f - wobble_depth;

    throbber.advance(throb_speed);
    wobbler.advance(wobble_speed, wobble_phase_offset);
    auto x = throb_depth*throbber.x() + wobble_depth*wobbler.x();
    auto y = throb_depth*throbber.y() + wobble_depth*wobbler.y();

    auto x_gain = gain(X_GAIN_KNOB, X_GAIN_CV);
    auto y_gain = gain(Y_GAIN_KNOB, Y_GAIN_CV);
    auto x_offset = param(X_RANGE_SWITCH)*5.f;
    auto y_offset = param(Y_RANGE_SWITCH)*5.f;
    outputs[X_OUT].value = 5.f*x_gain*(x + x_offset);
    outputs[Y_OUT].value = 5.f*y_gain*(y + y_offset);
  }

  enum ParameterIds {
    WOBBLE_RATIO_KNOB,
    WOBBLE_RATIO_CV_ATTENUVERTER,
    WOBBLE_TYPE_SWITCH,
    WOBBLE_DEPTH_KNOB,
    WOBBLE_DEPTH_CV_ATTENUVERTER,
    THROB_SPEED_KNOB,
    THROB_SPEED_CV_ATTENUVERTER,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    QUANTIZE_WOBBLE_RATIO_SWITCH,
    WOBBLE_PHASE_KNOB,
    PARAMETER_COUNT
  };
  enum InputIds {
    GEAR_RATIO_CV,
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
