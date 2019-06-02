#pragma once

#include <engine/Module.hpp>
#include <functional>

namespace dhe {

class Upstage : public rack::engine::Module {
public:
  Upstage();

  void process(const ProcessArgs &args) override;

  enum ParameterIds { LevelKnob, TriggerButton, WaitButton, LevelRangeSwitch, ParameterCount };

  enum InputIds { TriggerInput, WaitInput, LevelCvInput, InputCount };

  enum OutputIds { TriggerOutput, EnvelopeOutput, OutputCount };

private:
  void sendEnvelope(float voltage);
  void sendTrigger(bool isTriggered);
  auto triggerIn() -> bool;
  auto waitIn() -> bool;

  std::function<float()> level;
};

} // namespace dhe
