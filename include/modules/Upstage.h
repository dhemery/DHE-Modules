#pragma once

#include "modules/controls/Level.h"

#include <engine/Module.hpp>

namespace dhe {

class Upstage : public rack::engine::Module {
public:
  Upstage();

  void process(const ProcessArgs &args) override;

  enum ParameterIds { LevelKnob, TriggerButton, WaitButton, LevelRangeSwitch, ParameterCount };

  enum InputIds { TriggerInput, WaitInput, LevelCvInput, InputCount };

  enum OutputIds { TriggerOutput, EnvelopeOutput, OutputCount };

private:
  void sendEnvelope(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void sendTrigger(bool isTriggered) {
    const auto voltage = level::unipolarRange.scale(isTriggered);
    outputs[TriggerOutput].setVoltage(voltage);
  }

  auto triggerIn() -> bool {
    auto triggerButton = params[TriggerButton].getValue() > 0.1F;
    auto triggerInput = inputs[TriggerInput].getVoltage() > 0.1F;
    return triggerButton || triggerInput;
  }

  auto waitIn() -> bool {
    auto waitButton = params[WaitButton].getValue() > 0.1F;
    auto waitInput = inputs[WaitInput].getVoltage() > 0.1F;
    return waitButton || waitInput;
  }

  auto level() -> float { return scaledRotation<2>(this, LevelKnob, LevelCvInput, LevelRangeSwitch, level::ranges); }
};

} // namespace dhe
