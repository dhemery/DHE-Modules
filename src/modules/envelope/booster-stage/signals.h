#pragma once

#include "control-ids.h"

#include "components/sigmoid.h"
#include "signals/basic.h"
#include "signals/curvature.h"
#include "signals/durations.h"
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
    auto const rotation =
        rotation_of(params_[Param::Curvature], inputs_[Input::CurvatureCv]);
    return Curvature::curvature(rotation);
  }

  auto defer() const -> bool {
    return is_high(inputs_[Input::Defer]) || is_pressed(params_[Param::Defer]);
  }

  auto duration() const -> float {
    auto const rotation =
        rotation_of(params_[Param::Duration], inputs_[Input::DurationCv]);
    auto const range =
        value_of<Durations::Selection>(params_[Param::DurationRange]);
    return Durations::seconds(rotation, range);
  }

  auto gate() const -> bool {
    return is_high(inputs_[Input::Trigger]) ||
           is_pressed(params_[Param::Trigger]);
  }

  auto input() const -> float { return voltage_at(inputs_[Input::Envelope]); }

  auto level() const -> float {
    auto const rotation =
        rotation_of(params_[Param::Level], inputs_[Input::LevelCv]);
    auto const range = value_of<VoltageRange>(params_[Param::LevelRange]);
    return VoltageRanges::volts(rotation, range);
  }

  void output(float voltage) { outputs_[Output::Envelope].setVoltage(voltage); }

  void show_active(bool is_active) {
    auto const voltage = VoltageRanges::volts(is_active || active_button(),
                                              VoltageRange::Unipolar);
    outputs_[Output::Active].setVoltage(voltage);
  }

  void show_eoc(bool is_eoc) {
    auto const voltage =
        VoltageRanges::volts(is_eoc || active_button(), VoltageRange::Unipolar);
    outputs_[Output::Eoc].setVoltage(voltage);
  }

  auto taper() const -> sigmoid::Taper const & {
    auto const shape = value_of<Shapes::Selection>(params_[Param::Shape]);
    return Shapes::taper(shape);
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
