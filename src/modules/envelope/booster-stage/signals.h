#pragma once

#include "control-ids.h"

#include "components/sigmoid.h"
#include "signals/common-inputs.h"
#include "signals/durations.h"
#include "signals/shapes.h"
#include "signals/voltages.h"

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
    return Curvature::value(
        rotation(params_[Param::Curvature], inputs_[Input::CurvatureCv]));
  }

  auto defer() const -> bool {
    return is_high(inputs_[Input::Defer]) || is_pressed(params_[Param::Defer]);
  }

  auto duration() const -> float {
    return Durations::value(
        rotation(params_[Param::Duration], inputs_[Input::DurationCv]),
        position_of(params_[Param::DurationRange]));
  }

  auto gate() const -> bool {
    return is_high(inputs_[Input::Trigger]) ||
           is_pressed(params_[Param::Trigger]);
  }

  auto input() const -> float { return voltage_at(inputs_[Input::Envelope]); }

  auto level() const -> float {
    return Voltages::value(
        rotation(params_[Param::Level], inputs_[Input::LevelCv]),
        position_of(params_[Param::LevelRange]));
  }

  void output(float voltage) { outputs_[Output::Envelope].setVoltage(voltage); }

  void show_active(bool is_active) {
    auto const voltage =
        UnipolarVoltage::range.scale(is_active || active_button());
    outputs_[Output::Active].setVoltage(voltage);
  }

  void show_eoc(bool is_eoc) {
    auto const voltage = UnipolarVoltage::range.scale(is_eoc || eoc_button());
    outputs_[Output::Eoc].setVoltage(voltage);
  }

  auto taper() const -> sigmoid::Taper const & {
    return selected_taper(params_[Param::Shape]);
  }

private:
  auto active_button() const -> bool {
    return is_pressed(params_[Param::Active]);
  }

  auto eoc_button() const -> bool { return is_pressed(params_[Param::Eoc]); }

  std::vector<TParam> const &params_;
  std::vector<TInput> const &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace booster_stage
} // namespace envelope
} // namespace dhe
