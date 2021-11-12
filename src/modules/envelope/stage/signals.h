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
namespace stage {

template <typename TParam, typename TInput, typename TOutput> struct Signals {
  Signals(std::vector<TParam> &params, std::vector<TInput> &inputs,
          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {}

  auto curvature() const -> float {
    return Curvature::curvature(value_of(params_[Param::Curvature]));
  }

  auto defer() const -> bool { return is_high(inputs_[Input::Defer]); }

  auto duration() const -> float {
    return MediumDuration::seconds(value_of(params_[Param::Duration]));
  }

  auto gate() const -> bool { return is_high(inputs_[Input::Trigger]); }

  auto input() const -> float { return voltage_at(inputs_[Input::Envelope]); }

  auto level() const -> float {
    auto const rotation = rotation_of(params_[Param::Level]);
    return UnipolarVoltage::scale(rotation);
  }

  void output(float voltage) { outputs_[Output::Envelope].setVoltage(voltage); }

  void show_active(bool active) {
    auto const voltage = UnipolarVoltage::scale(active);
    outputs_[Output::Active].setVoltage(voltage);
  }

  void show_eoc(bool eoc) {
    auto const voltage = UnipolarVoltage::scale(eoc);
    outputs_[Output::Eoc].setVoltage(voltage);
  }

  static auto shape() -> Shape { return Shape::J; };

private:
  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace stage
} // namespace envelope
} // namespace dhe
