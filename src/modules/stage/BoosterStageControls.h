#pragma once

#include "components/taper.h"
#include "controls/common-inputs.h"
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

class BoosterStageControls {
public:
  BoosterStageControls(std::vector<Input> const &inputs,
                       std::vector<Param> const &params,
                       std::vector<Output> &outputs)
      : inputs{inputs}, params{params}, outputs{outputs} {}

  auto curvature() const -> float {
    return dhe::curvature(params[CurveKnob], inputs[CurveCvInput]);
  }

  auto defer() const -> bool {
    return is_high(inputs[DeferInput]) || is_pressed(params[DeferButton]);
  }

  auto duration() const -> float {
    return selectable_duration(params[DurationKnob], inputs[DurationCvInput],
                               params[DurationRangeSwitch]);
  }

  auto gate() const -> bool {
    return is_high(inputs[TriggerInput]) || is_pressed(params[TriggerButton]);
  }

  auto input() const -> float { return voltage_at(inputs[EnvelopeInput]); }

  auto level() const -> float {
    return selectable_level(params[LevelKnob], inputs[LevelCvInput],
                            params[LevelRangeSwitch]);
  }

  void output(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void showActive(bool isActive) {
    auto const voltage =
        unipolar_signal_range.scale(isActive || activeButton());
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void showEoc(bool isEoc) {
    auto const voltage = unipolar_signal_range.scale(isEoc || eocButton());
    outputs[EocOutput].setVoltage(voltage);
  }

  auto taper() const -> taper::VariableTaper const * {
    return selected_taper(params[ShapeSwitch]);
  }

  enum ParameterIds {
    ActiveButton,
    CurveKnob,
    DeferButton,
    DurationKnob,
    DurationRangeSwitch,
    EocButton,
    LevelKnob,
    LevelRangeSwitch,
    ShapeSwitch,
    TriggerButton,
    ParameterCount
  };

  enum InputIds {
    CurveCvInput,
    DeferInput,
    DurationCvInput,
    LevelCvInput,
    EnvelopeInput,
    TriggerInput,
    InputCount
  };

  enum OutputIds { ActiveOutput, EocOutput, EnvelopeOutput, OutputCount };

private:
  auto activeButton() const -> bool { return is_pressed(params[ActiveButton]); }

  auto eocButton() const -> bool { return is_pressed(params[EocButton]); }

  std::vector<Input> const &inputs;
  std::vector<Param> const &params;
  std::vector<Output> &outputs;
};
} // namespace stage
} // namespace dhe
