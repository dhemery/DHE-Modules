#pragma once

#include "control-ids.h"

#include "components/cxmath.h"
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
        rotation_of(params_[ParamId::Curvature], inputs_[InputId::CurvatureCv]);
    return Curvature::scale(Rotation::clamp(rotation));
  }

  auto defer() const -> bool {
    return is_high(inputs_[InputId::Defer]) ||
           is_pressed(params_[ParamId::Defer]);
  }

  auto duration() const -> float {
    auto const rotation =
        rotation_of(params_[ParamId::Duration], inputs_[InputId::DurationCv]);
    auto const range =
        value_of<DurationRangeId>(params_[ParamId::DurationRange]);
    return Duration::scale(cx::max(rotation, 0.F), range);
  }

  auto gate() const -> bool {
    return is_high(inputs_[InputId::Trigger]) ||
           is_pressed(params_[ParamId::Trigger]);
  }

  auto input() const -> float { return voltage_at(inputs_[InputId::Envelope]); }

  auto level() const -> float {
    auto const rotation =
        rotation_of(params_[ParamId::Level], inputs_[InputId::LevelCv]);
    auto const range_id =
        value_of<VoltageRangeId>(params_[ParamId::LevelRange]);
    return Voltage::scale(rotation, range_id);
  }

  void output(float voltage) {
    outputs_[OutputId::Envelope].setVoltage(voltage);
  }

  void show_active(bool is_active) {
    auto const voltage =
        Voltage::scale(is_active || active_button(), VoltageRangeId::Unipolar);
    outputs_[OutputId::Active].setVoltage(voltage);
  }

  void show_eoc(bool is_eoc) {
    auto const voltage =
        Voltage::scale(is_eoc || active_button(), VoltageRangeId::Unipolar);
    outputs_[OutputId::Eoc].setVoltage(voltage);
  }

  auto shape() const -> Shape::Id {
    return value_of<Shape::Id>(params_[ParamId::Shape]);
  }

private:
  auto active_button() const -> bool {
    return is_pressed(params_[ParamId::Active]);
  }

  auto eoc_button() const -> bool { return is_pressed(params_[ParamId::Eoc]); }

  std::vector<TParam> const &params_;
  std::vector<TInput> const &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace booster_stage
} // namespace envelope
} // namespace dhe
