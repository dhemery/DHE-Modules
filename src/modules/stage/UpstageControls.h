#pragma once

#include "config/CommonConfig.h"
#include "config/LevelConfig.h"
#include "controls/CommonInputs.h"
#include "controls/LevelInputs.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <vector>

namespace dhe {
namespace stage {
using rack::engine::Input;
using rack::engine::Output;
using rack::engine::Param;

class UpstageControls {
public:
  UpstageControls(std::vector<Input> const &inputs,
                  std::vector<Param> const &params,
                  std::vector<Output> &outputs)
      : inputs{inputs}, params{params}, outputs{outputs} {}

  auto isTriggered() const -> bool {
    return is_high(inputs[TriggerInput]) || is_pressed(params[TriggerButton]);
  }

  auto isWaiting() const -> bool {
    return is_high(inputs[WaitInput]) || is_pressed(params[WaitButton]);
  }

  auto level() const -> float {
    return selectable_level(params[LevelKnob], inputs[LevelCvInput],
                            params[LevelRangeSwitch]);
  }

  void sendEnvelope(float voltage) {
    outputs[EnvelopeOutput].setVoltage(voltage);
  }

  void sendTrigger(bool isTriggered) {
    auto const voltage = unipolar_signal_range.scale(isTriggered);
    outputs[TriggerOutput].setVoltage(voltage);
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
  std::vector<Input> const &inputs;
  std::vector<Param> const &params;
  std::vector<Output> &outputs;
};
} // namespace stage

} // namespace dhe
