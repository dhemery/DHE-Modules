#pragma once

#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace stage {
using rack::engine::Input;
using rack::engine::Output;
using rack::engine::Param;

class StageControls {
public:
  StageControls(std::vector<Input> const &inputs,
                std::vector<Param> const &params, std::vector<Output> &outputs)
      : inputs{inputs}, params{params}, outputs{outputs} {}

  auto curvature() const -> float { return dhe::curvature(params[CurveKnob]); }

  auto defer() const -> bool { return is_high(inputs[DeferInput]); }

  auto duration() const -> float {
    return dhe::duration(params[DurationKnob], medium_duration_range);
  }

  auto gate() const -> bool { return is_high(inputs[TriggerInput]); }

  auto input() const -> float { return voltage_at(inputs[EnvelopeInput]); }

  auto level() const -> float {
    return dhe::level(params[LevelKnob], unipolar_signal_range);
  }

  void output(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void showActive(bool active) {
    auto const voltage = unipolar_signal_range.scale(active);
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void showEoc(bool eoc) {
    auto const voltage = unipolar_signal_range.scale(eoc);
    outputs[EocOutput].setVoltage(voltage);
  }

  static auto taper() -> taper::VariableTaper const * {
    return &taper::variable_j_taper;
  };

  enum ParameterIds { DurationKnob, LevelKnob, CurveKnob, ParameterCount };

  enum InputIds { EnvelopeInput, TriggerInput, DeferInput, InputCount };

  enum OutputIds { EnvelopeOutput, EocOutput, ActiveOutput, OutputCount };

private:
  std::vector<Input> const &inputs;
  std::vector<Param> const &params;
  std::vector<Output> &outputs;
};
} // namespace stage
} // namespace dhe
