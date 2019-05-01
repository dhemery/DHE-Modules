#pragma once

#include "Module.h"

#include "components/rotor.h"

namespace DHE {

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
  auto bounce() const -> float;
  auto depth() const -> float;
  auto is_bounce_free() const -> bool;
  auto offset(int param) const -> float;
  auto phase() const -> float;
  auto spin(float sample_time) const -> float;
  auto x_gain_in() const -> float;
  auto x_offset() const -> float;
  auto y_gain_in() const -> float;
  auto y_offset() const -> float;

  static constexpr auto speed_curvature = 0.8f;
  Rotor spinner{};
  Rotor bouncer{};
};
} // namespace DHE
