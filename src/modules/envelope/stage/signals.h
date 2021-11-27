#pragma once

#include "control-ids.h"

#include "components/sigmoid.h"
#include "signals/basic.h"
#include "signals/curvature-signals.h"
#include "signals/duration-signals.h"
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
    return Curvature::scale(value_of(params_[ParamId::Curvature]));
  }

  auto defer() const -> bool { return is_high(inputs_[InputId::Defer]); }

  auto duration() const -> float {
    return MediumDuration::scale(value_of(params_[ParamId::Duration]));
  }

  auto gate() const -> bool { return is_high(inputs_[InputId::Trigger]); }

  auto input() const -> float { return voltage_at(inputs_[InputId::Envelope]); }

  auto level() const -> float {
    auto const rotation = rotation_of(params_[ParamId::Level]);
    return UnipolarVoltage::scale(rotation);
  }

  void output(float voltage) {
    outputs_[OutputId::Envelope].setVoltage(voltage);
  }

  void show_active(bool active) {
    auto const voltage = UnipolarVoltage::scale(active);
    outputs_[OutputId::Active].setVoltage(voltage);
  }

  void show_eoc(bool eoc) {
    auto const voltage = UnipolarVoltage::scale(eoc);
    outputs_[OutputId::Eoc].setVoltage(voltage);
  }

  static auto shape() -> Shape::Id { return Shape::Id::J; };

private:
  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace stage
} // namespace envelope
} // namespace dhe
