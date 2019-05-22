#pragma once

#include "Module.h"

#include "modules/components/Rotor.h"

namespace dhe {

class Blossom : public Module {
public:
  Blossom();
  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    SPIN_KNOB,
    SPIN_AV,
    BOUNCE_KNOB,
    BOUNCE_AV,
    BOUNCE_LOCK_SWITCH,
    DEPTH_KNOB,
    DEPTH_AV,
    PHASE_KNOB,
    PHASE_AV,
    X_GAIN_KNOB,
    Y_GAIN_KNOB,
    X_RANGE_SWITCH,
    Y_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds {
    SPIN_CV,
    BOUNCE_CV,
    DEPTH_CV,
    PHASE_CV,
    X_GAIN_CV,
    Y_GAIN_CV,
    INPUT_COUNT
  };
  enum OutputIds { X_OUT, Y_OUT, OUTPUT_COUNT };

private:
  auto bounce() -> float;
  auto depth() -> float;
  auto is_bounce_free() -> bool;
  auto offset(int param) -> float;
  auto phase() -> float;
  auto spin(float sample_time) -> float;
  auto x_gain_in() -> float;
  auto x_offset() -> float;
  auto y_gain_in() -> float;
  auto y_offset() -> float;

  static constexpr auto speed_curvature = 0.8f;
  Rotor spinner{};
  Rotor bouncer{};
};
} // namespace dhe
