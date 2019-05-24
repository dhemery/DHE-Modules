#pragma once

#include "Module.h"

#include <functional>

namespace dhe {

class Upstage : public Module {
public:
  Upstage();

  void process(const ProcessArgs &args) override;

  enum ParameterIds { LEVEL_KNOB, TRIGGER_BUTTON, WAIT_BUTTON, LEVEL_RANGE_SWITCH, PARAMETER_COUNT };

  enum InputIds { TRIGGER_IN, WAIT_IN, LEVEL_CV, INPUT_COUNT };

  enum OutputIds { TRIGGER_OUT, MAIN_OUT, OUTPUT_COUNT };

private:
  void sendEnvelope(float voltage);
  void sendTrigger(bool isTriggered);
  auto triggerIn() -> bool;
  auto waitIn() -> bool;

  std::function<float()> level;
};

} // namespace dhe
