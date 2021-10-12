#pragma once

#include "components/sigmoid.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"

#include "rack.hpp"

#include <vector>

namespace dhe {
namespace stage {

class StageControls {
  using Input = rack::engine::Input;
  using Output = rack::engine::Output;
  using Param = rack::engine::Param;

public:
  StageControls(std::vector<Input> const &inputs,
                std::vector<Param> const &params, std::vector<Output> &outputs)
      : inputs_{inputs}, params_{params}, outputs_{outputs} {}

  auto curvature() const -> float { return dhe::curvature(params_[CurveKnob]); }

  auto defer() const -> bool { return is_high(inputs_[DeferInput]); }

  auto duration() const -> float {
    return dhe::duration(params_[DurationKnob], medium_duration_range);
  }

  auto gate() const -> bool { return is_high(inputs_[TriggerInput]); }

  auto input() const -> float { return voltage_at(inputs_[EnvelopeInput]); }

  auto level() const -> float {
    return dhe::level(params_[LevelKnob], unipolar_signal_range);
  }

  void output(float voltage) { outputs_[EnvelopeOutput].setVoltage(voltage); }

  void show_active(bool active) {
    auto const voltage = unipolar_signal_range.scale(active);
    outputs_[ActiveOutput].setVoltage(voltage);
  }

  void show_eoc(bool eoc) {
    auto const voltage = unipolar_signal_range.scale(eoc);
    outputs_[EocOutput].setVoltage(voltage);
  }

  static auto taper() -> sigmoid::Taper const & { return sigmoid::j_taper; };

  enum ParameterIds { DurationKnob, LevelKnob, CurveKnob, ParameterCount };

  enum InputIds { EnvelopeInput, TriggerInput, DeferInput, InputCount };

  enum OutputIds { EnvelopeOutput, EocOutput, ActiveOutput, OutputCount };

private:
  std::vector<Input> const &inputs_;
  std::vector<Param> const &params_;
  std::vector<Output> &outputs_;
};
} // namespace stage
} // namespace dhe
