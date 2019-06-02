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

  auto triggerIn() const -> bool { return inputIsHigh(this, TriggerInput) || buttonIsPressed(this, TriggerButton); }

  auto waitIn() const -> bool { return inputIsHigh(this, WaitInput) || buttonIsPressed(this, WaitButton); }

  auto level() const -> float {
    return scaledRotation<2>(this, LevelKnob, LevelCvInput, LevelRangeSwitch, level::ranges);
  }
};

} // namespace dhe
