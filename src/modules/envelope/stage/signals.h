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
namespace stage {

template <typename TParam, typename TInput, typename TOutput> struct Signals {
  Signals(std::vector<TParam> &params, std::vector<TInput> &inputs,
          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {}

  auto curvature() const -> float {
    return Curvature::value(rotation_of(params_[Param::Curvature]));
  }

  auto defer() const -> bool { return is_high(inputs_[Input::Defer]); }

  auto duration() const -> float {
    return MediumDuration::value(rotation_of(params_[Param::Duration]));
  }

  auto gate() const -> bool { return is_high(inputs_[Input::Trigger]); }

  auto input() const -> float { return voltage_at(inputs_[Input::Envelope]); }

  auto level() const -> float {
    return UnipolarVoltage::value(rotation_of(params_[Param::Level]));
  }

  void output(float voltage) { outputs_[Output::Envelope].setVoltage(voltage); }

  void show_active(bool active) {
    auto const voltage = UnipolarVoltage::range.scale(active);
    outputs_[Output::Active].setVoltage(voltage);
  }

  void show_eoc(bool eoc) {
    auto const voltage = UnipolarVoltage::range.scale(eoc);
    outputs_[Output::Eoc].setVoltage(voltage);
  }

  static auto taper() -> sigmoid::Taper const & { return sigmoid::j_taper; };

private:
  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace stage
} // namespace envelope
} // namespace dhe
