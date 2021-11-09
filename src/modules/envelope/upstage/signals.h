#pragma once

#include "signals/basic.h"
#include "signals/voltages.h"

#include <vector>

namespace dhe {
namespace envelope {
namespace upstage {

template <typename TParam, typename TInput, typename TOutput> struct Signals {
  Signals(std::vector<TParam> &params, std::vector<TInput> &inputs,

          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {}

  auto is_triggered() const -> bool {
    return is_high(inputs_[Input::Trigger]) ||
           is_pressed(params_[Param::Trigger]);
  }

  auto is_waiting() const -> bool {
    return is_high(inputs_[Input::Wait]) || is_pressed(params_[Param::Wait]);
  }

  auto level() const -> float {
    return Voltages::value(
        rotation(params_[Param::Level], inputs_[Input::LevelCv]),
        position_of(params_[Param::LevelRange]));
  }

  void send_envelope(float voltage) {
    outputs_[Output::Envelope].setVoltage(voltage);
  }

  void send_trigger(bool is_triggered) {
    auto const voltage = UnipolarVoltage::range().scale(is_triggered);
    outputs_[Output::Trigger].setVoltage(voltage);
  }

private:
  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace upstage
} // namespace envelope

} // namespace dhe
