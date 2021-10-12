#pragma once

#include "components/sigmoid.h"
#include "controls/common-inputs.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"

#include <rack.hpp>

#include <vector>

namespace dhe {
namespace stage {
class BoosterStageControls {
  using Input = rack::engine::Input;
  using Output = rack::engine::Output;
  using Param = rack::engine::Param;

public:
  BoosterStageControls(std::vector<Input> const &inputs,
                       std::vector<Param> const &params,
                       std::vector<Output> &outputs)
      : inputs_{inputs}, params_{params}, outputs_{outputs} {}

  auto curvature() const -> float {
    return dhe::curvature(params_[CurveKnob], inputs_[CurveCvInput]);
  }

  auto defer() const -> bool {
    return is_high(inputs_[DeferInput]) || is_pressed(params_[DeferButton]);
  }

  auto duration() const -> float {
    return selectable_duration(params_[DurationKnob], inputs_[DurationCvInput],
                               params_[DurationRangeSwitch]);
  }

  auto gate() const -> bool {
    return is_high(inputs_[TriggerInput]) || is_pressed(params_[TriggerButton]);
  }

  auto input() const -> float { return voltage_at(inputs_[EnvelopeInput]); }

  auto level() const -> float {
    return selectable_level(params_[LevelKnob], inputs_[LevelCvInput],
                            params_[LevelRangeSwitch]);
  }

  void output(float voltage) { outputs_[EnvelopeOutput].setVoltage(voltage); }

  void show_active(bool is_active) {
    auto const voltage =
        unipolar_signal_range.scale(is_active || active_button());
    outputs_[ActiveOutput].setVoltage(voltage);
  }

  void show_eoc(bool is_eoc) {
    auto const voltage = unipolar_signal_range.scale(is_eoc || eoc_button());
    outputs_[EocOutput].setVoltage(voltage);
  }

  auto taper() const -> sigmoid::Taper const & {
    return selected_taper(params_[ShapeSwitch]);
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
  auto active_button() const -> bool {
    return is_pressed(params_[ActiveButton]);
  }

  auto eoc_button() const -> bool { return is_pressed(params_[EocButton]); }

  std::vector<Input> const &inputs_;
  std::vector<Param> const &params_;
  std::vector<Output> &outputs_;
};
} // namespace stage
} // namespace dhe
