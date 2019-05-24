#include "modules/Upstage.h"

#include "modules/controls/Level.h"

namespace dhe {

Upstage::Upstage() {
  config(ParameterCount, InputCount, OutputCount);

  level::configKnob(this, LevelKnob, LevelRangeSwitch);
  level::configSwitch(this, LevelRangeSwitch);
  level = level::withSelectableRange(this, LevelKnob, LevelCvInput, LevelRangeSwitch);

  configParam(TriggerButton, 0.f, 1.f, 0.f, "Trigger");
  configParam(WaitButton, 0.f, 1.f, 0.f, "Wait");
}

void Upstage::process(const ProcessArgs & /*args*/) {
  auto isTriggered = triggerIn() && !waitIn();
  sendTrigger(isTriggered);
  sendEnvelope(level());
}

void Upstage::sendEnvelope(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

void Upstage::sendTrigger(bool isTriggered) {
  const auto voltage = isTriggered ? 10.f : 0.f;
  outputs[TriggerOutput].setVoltage(voltage);
}

auto Upstage::triggerIn() -> bool {
  auto triggerButton = params[TriggerButton].getValue() > 0.1f;
  auto triggerInput = inputs[TriggerInput].getVoltage() > 0.1f;
  return triggerButton || triggerInput;
}

auto Upstage::waitIn() -> bool {
  auto waitButton = params[WaitButton].getValue() > 0.1f;
  auto waitInput = inputs[WaitInput].getVoltage() > 0.1f;
  return waitButton || waitInput;
}

} // namespace dhe
