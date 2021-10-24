#pragma once

#include "control-ids.h"

#include "modules/envelope/mode/mode.h"
#include "signals/duration-inputs.h"

#include <vector>

namespace dhe {
namespace envelope {
namespace hostage {

template <typename TParam, typename TInput, typename TOutput> struct Signals {
  using Mode = mode::Mode;

  Signals(std::vector<TParam> const &params, std::vector<TInput> const &inputs,
          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {};

  auto defer() const -> bool { return is_high(inputs_[Input::DeferInput]); }

  auto duration() const -> float {
    return selectable_duration(params_[Param::DurationKnob],
                               inputs_[Input::DurationCvInput],
                               params_[Param::DurationRangeSwitch]);
  }

  auto gate() const -> bool { return is_high(inputs_[Input::TriggerInput]); }

  auto input() const -> float {
    return voltage_at(inputs_[Input::EnvelopeInput]);
  }

  auto mode() const -> Mode {
    return position_of(params_[Param::ModeSwitch]) == 1 ? Mode::Sustain
                                                        : Mode::Hold;
  }

  void output(float voltage) {
    outputs_[Output::EnvelopeOutput].setVoltage(voltage);
  }

  void show_active(bool active) {
    auto const voltage = unipolar_signal_range.scale(active);
    outputs_[Output::ActiveOutput].setVoltage(voltage);
  }

  void show_eoc(bool eoc) {
    auto const voltage = unipolar_signal_range.scale(eoc);
    outputs_[Output::EocOutput].setVoltage(voltage);
  }

private:
  std::vector<TParam> const &params_;
  std::vector<TInput> const &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace hostage
} // namespace envelope
} // namespace dhe
