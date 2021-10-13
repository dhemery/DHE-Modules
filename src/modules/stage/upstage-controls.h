#pragma once

#include "config/common-config.h"
#include "config/level-config.h"
#include "signals/common-inputs.h"
#include "signals/level-inputs.h"

#include "rack.hpp"

#include <vector>

namespace dhe {
namespace stage {

class UpstageControls {
  using Input = rack::engine::Input;
  using Output = rack::engine::Output;
  using Param = rack::engine::Param;

public:
  UpstageControls(std::vector<Input> const &inputs,
                  std::vector<Param> const &params,
                  std::vector<Output> &outputs)
      : inputs_{inputs}, params_{params}, outputs_{outputs} {}

  auto is_triggered() const -> bool {
    return is_high(inputs_[TriggerInput]) || is_pressed(params_[TriggerButton]);
  }

  auto is_waiting() const -> bool {
    return is_high(inputs_[WaitInput]) || is_pressed(params_[WaitButton]);
  }

  auto level() const -> float {
    return selectable_level(params_[LevelKnob], inputs_[LevelCvInput],
                            params_[LevelRangeSwitch]);
  }

  void send_envelope(float voltage) {
    outputs_[EnvelopeOutput].setVoltage(voltage);
  }

  void send_trigger(bool is_triggered) {
    auto const voltage = unipolar_signal_range.scale(is_triggered);
    outputs_[TriggerOutput].setVoltage(voltage);
  }

  enum ParameterIds {
    LevelKnob,
    TriggerButton,
    WaitButton,
    LevelRangeSwitch,
    ParameterCount
  };

  enum InputIds { TriggerInput, WaitInput, LevelCvInput, InputCount };

  enum OutputIds { TriggerOutput, EnvelopeOutput, OutputCount };

private:
  std::vector<Input> const &inputs_;
  std::vector<Param> const &params_;
  std::vector<Output> &outputs_;
};
} // namespace stage

} // namespace dhe
