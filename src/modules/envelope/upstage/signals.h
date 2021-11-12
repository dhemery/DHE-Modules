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
    return is_high(inputs_[InputId::Trigger]) ||
           is_pressed(params_[ParamId::Trigger]);
  }

  auto is_waiting() const -> bool {
    return is_high(inputs_[InputId::Wait]) ||
           is_pressed(params_[ParamId::Wait]);
  }

  auto level() const -> float {
    auto const rotation =
        rotation_of(params_[ParamId::Level], inputs_[InputId::LevelCv]);
    auto const range_id =
        value_of<VoltageRangeId>(params_[ParamId::LevelRange]);
    return VoltageRanges::scale(rotation, range_id);
  }

  void send_envelope(float voltage) {
    outputs_[OutputId::Envelope].setVoltage(voltage);
  }

  void send_trigger(bool is_triggered) {
    auto const voltage = UnipolarVoltage::range().scale(is_triggered);
    outputs_[OutputId::Trigger].setVoltage(voltage);
  }

private:
  std::vector<TParam> &params_;
  std::vector<TInput> &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace upstage
} // namespace envelope

} // namespace dhe
