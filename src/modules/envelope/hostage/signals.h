#pragma once

#include "control-ids.h"

#include "modules/envelope/mode/mode.h"
#include "signals/durations.h"

#include <vector>

namespace dhe {
namespace envelope {
namespace hostage {

template <typename TParam, typename TInput, typename TOutput> struct Signals {
  using Mode = mode::Mode;

  Signals(std::vector<TParam> const &params, std::vector<TInput> const &inputs,
          std::vector<TOutput> &outputs)
      : params_{params}, inputs_{inputs}, outputs_{outputs} {};

  auto defer() const -> bool { return is_high(inputs_[Input::Defer]); }

  auto duration() const -> float {
    auto const rotation =
        rotation_of(params_[Param::Duration], inputs_[Input::DurationCv]);
    auto const range = value_of<DurationRangeId>(params_[Param::DurationRange]);
    return DurationRanges::seconds(rotation, range);
  }

  auto gate() const -> bool { return is_high(inputs_[Input::Trigger]); }

  auto input() const -> float { return voltage_at(inputs_[Input::Envelope]); }

  auto mode() const -> Mode {
    return is_pressed(params_[Param::Mode]) ? Mode::Sustain : Mode::Hold;
  }

  void output(float voltage) { outputs_[Output::Envelope].setVoltage(voltage); }

  void show_active(bool active) {
    auto const voltage = UnipolarVoltage::volts((float)active);
    outputs_[Output::Active].setVoltage(voltage);
  }

  void show_eoc(bool eoc) {
    auto const voltage = UnipolarVoltage::volts((float)eoc);
    outputs_[Output::Eoc].setVoltage(voltage);
  }

private:
  std::vector<TParam> const &params_;
  std::vector<TInput> const &inputs_;
  std::vector<TOutput> &outputs_;
};
} // namespace hostage
} // namespace envelope
} // namespace dhe
