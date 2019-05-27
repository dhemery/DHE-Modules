#include "modules/Upstage.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

Upstage::Upstage() {
  config(ParameterCount, InputCount, OutputCount);

  level::configKnob(this, LevelKnob, LevelRangeSwitch);
  level::configSwitch(this, LevelRangeSwitch);
  level = level::withSelectableRange(this, LevelKnob, LevelCvInput, LevelRangeSwitch);

  button::config(this, TriggerButton, "TRIG", {"From input", "High"}, 0);
  button::config(this, WaitButton, "WAIT", {"From input", "High"}, 0);
}

void Upstage::process(const ProcessArgs & /*args*/) {
  auto isTriggered = triggerIn() && !waitIn();
  sendTrigger(isTriggered);
  sendEnvelope(level());
}

void Upstage::sendEnvelope(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

void Upstage::sendTrigger(bool isTriggered) {
  const auto voltage = level::unipolarRange.scale(isTriggered);
  outputs[TriggerOutput].setVoltage(voltage);
}

auto Upstage::triggerIn() -> bool {
  auto triggerButton = params[TriggerButton].getValue() > 0.1F;
  auto triggerInput = inputs[TriggerInput].getVoltage() > 0.1F;
  return triggerButton || triggerInput;
}

auto Upstage::waitIn() -> bool {
  auto waitButton = params[WaitButton].getValue() > 0.1F;
  auto waitInput = inputs[WaitInput].getVoltage() > 0.1F;
  return waitButton || waitInput;
}

} // namespace dhe
