#pragma once

#include "control-ids.h"

#include "components/sigmoid.h"
#include "signals/common-inputs.h"
#include "signals/curvature-inputs.h"
#include "signals/duration-inputs.h"
#include "signals/level-inputs.h"

#include <vector>

namespace dhe {
namespace envelope {
namespace booster_stage {

template <typename TParam, typename TInput, typename TOutput> struct Signals {

public:
  Signals(std::vector<TParam> const &params, std::vector<TInput> const &inputs,
          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {}

  auto curvature() const -> float {
    return dhe::curvature(params_[Param::CurveKnob],
                          inputs_[Input::CurveCvInput]);
  }

  auto defer() const -> bool {
    return is_high(inputs_[Input::DeferInput]) ||
           is_pressed(params_[Param::DeferButton]);
  }

  auto duration() const -> float {
    return selectable_duration(params_[Param::DurationKnob],
                               inputs_[Input::DurationCvInput],
                               params_[Param::DurationRangeSwitch]);
  }

  auto gate() const -> bool {
    return is_high(inputs_[Input::TriggerInput]) ||
           is_pressed(params_[Param::TriggerButton]);
  }

  auto input() const -> float {
    return voltage_at(inputs_[Input::EnvelopeInput]);
  }

  auto level() const -> float {
    return selectable_level(params_[Param::LevelKnob],
                            inputs_[Input::LevelCvInput],
                            params_[Param::LevelRangeSwitch]);
  }

  void output(float voltage) {
    outputs_[Output::EnvelopeOutput].setVoltage(voltage);
  }

  void show_active(bool is_active) {
    auto const voltage =
        unipolar_signal_range.scale(is_active || active_button());
    outputs_[Output::ActiveOutput].setVoltage(voltage);
  }

  void show_eoc(bool is_eoc) {
    auto const voltage = unipolar_signal_range.scale(is_eoc || eoc_button());
    outputs_[Output::EocOutput].setVoltage(voltage);
  }

  auto taper() const -> sigmoid::Taper const & {
    return selected_taper(params_[Param::ShapeSwitch]);
  }

private:
  auto active_button() const -> bool {
    return is_pressed(params_[Param::ActiveButton]);
  }

  auto eoc_button() const -> bool {
    return is_pressed(params_[Param::EocButton]);
  }

  std::vector<TParam> const &params_;
  std::vector<TInput> const &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace booster_stage
} // namespace envelope
} // namespace dhe
