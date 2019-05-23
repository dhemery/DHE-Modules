#pragma once

#include "Module.h"
#include "modules/components/Rotor.h"
#include "util/range.h"

namespace dhe {

class Xycloid : public Module {
public:
  Xycloid();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    WOBBLE_RATIO_KNOB,
    WOBBLE_RATIO_AV,
    WOBBLE_RANGE_SWITCH,
    WOBBLE_DEPTH_KNOB,
    WOBBLE_DEPTH_AV,
    THROB_SPEED_KNOB,
    THROB_SPEED_AV,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    WOBBLE_RATIO_FREEDOM_SWITCH,
    WOBBLE_PHASE_KNOB,
    PARAMETER_COUNT
  };
  enum InputIds {
    WOBBLE_RATIO_CV,
    WOBBLE_DEPTH_CV,
    THROB_SPEED_CV,
    X_GAIN_CV,
    Y_GAIN_CV,
    INPUT_COUNT
  };
  enum OutputIds { X_OUT, Y_OUT, OUTPUT_COUNT };

private:
  auto is_wobble_ratio_free() -> bool;
  auto offset(int param) -> float;
  auto throb_speed(float sampleTime) -> float;
  auto wobble_depth() -> float;
  auto wobble_phase_in() -> float;
  auto wobble_ratio_range() -> const Range &;
  auto wobble_ratio() -> float;
  auto x_offset() -> float;
  auto x_gain_in() -> float;
  auto y_gain_in() -> float;
  auto y_offset() -> float;

  static constexpr auto throb_speed_knob_range = Range{-1.f, 1.f};
  static constexpr auto wobble_depth_range = Range{0.f, 1.f};

  float wobble_ratio_offset{0.f};
  Rotor wobbler{};
  Rotor throbber{};
};
} // namespace dhe
