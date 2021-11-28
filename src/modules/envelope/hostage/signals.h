#pragma once

#include "control-ids.h"

#include "modules/envelope/mode/mode-ids.h"
#include "signals/basic.h"
#include "signals/duration-signals.h"
#include "signals/voltage-signals.h"

#include <vector>

namespace dhe {
namespace envelope {
namespace hostage {

template <typename TParam, typename TInput, typename TOutput> struct Signals {
  Signals(std::vector<TParam> const &params, std::vector<TInput> const &inputs,
          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {};

  auto defer() const -> bool { return is_high(inputs_[InputId::Defer]); }

  auto duration() const -> float {
    auto const rotation =
        rotation_of(params_[ParamId::Duration], inputs_[InputId::DurationCv]);
    auto const range =
        value_of<DurationRangeId>(params_[ParamId::DurationRange]);
    return Duration::scale(rotation, range);
  }

  auto gate() const -> bool { return is_high(inputs_[InputId::Trigger]); }

  auto input() const -> float { return voltage_at(inputs_[InputId::Envelope]); }

  auto mode() const -> envelope::ModeId {
    return is_pressed(params_[ParamId::Mode]) ? envelope::ModeId::Sustain
                                              : envelope::ModeId::Hold;
  }

  void output(float voltage) {
    outputs_[OutputId::Envelope].setVoltage(voltage);
  }

  void show_active(bool active) {
    auto const voltage = UnipolarVoltage::scale(static_cast<float>(active));
    outputs_[OutputId::Active].setVoltage(voltage);
  }

  void show_eoc(bool eoc) {
    auto const voltage = UnipolarVoltage::scale(static_cast<float>(eoc));
    outputs_[OutputId::Eoc].setVoltage(voltage);
  }

private:
  std::vector<TParam> const &params_;
  std::vector<TInput> const &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace hostage
} // namespace envelope
} // namespace dhe
