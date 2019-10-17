#pragma once

#include "modules/controls/CommonInputs.h"
#include "modules/controls/LevelInputs.h"

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
    const auto voltage = unipolarSignalRange.scale(isTriggered);
    outputs[TriggerOutput].setVoltage(voltage);
  }

  auto triggerIn() const -> bool { return inputIsHigh(inputs[TriggerInput]) || buttonIsPressed(params[TriggerButton]); }

  auto waitIn() const -> bool { return inputIsHigh(inputs[WaitInput]) || buttonIsPressed(params[WaitButton]); }

  auto level() const -> float {
    return selectableLevel(params[LevelKnob], inputs[LevelCvInput], params[LevelRangeSwitch]);
  }
};

} // namespace dhe
