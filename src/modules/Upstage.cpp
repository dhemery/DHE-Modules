#include "modules/Upstage.h"

#include "modules/controls/Level.h"

namespace dhe {

Upstage::Upstage() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  level::configKnob(this, LEVEL_KNOB, LEVEL_RANGE_SWITCH);
  level::configSwitch(this, LEVEL_RANGE_SWITCH);
  level = level::withSelectableRange(this, LEVEL_KNOB, LEVEL_CV, LEVEL_RANGE_SWITCH);

  configParam(TRIGGER_BUTTON, 0.f, 1.f, 0.f, "Trigger");
  configParam(WAIT_BUTTON, 0.f, 1.f, 0.f, "Wait");
}

void Upstage::process(const ProcessArgs &args) {
  auto isTriggered = triggerIn() && !waitIn();
  sendTrigger(isTriggered);
  sendEnvelope(level());
}

void Upstage::sendEnvelope(float voltage) { outputs[MAIN_OUT].setVoltage(voltage); }

void Upstage::sendTrigger(bool isTriggered) {
  const auto voltage = isTriggered ? 10.f : 0.f;
  outputs[TRIGGER_OUT].setVoltage(voltage);
}

auto Upstage::triggerIn() -> bool {
  auto triggerButton = params[TRIGGER_BUTTON].getValue() > 0.1f;
  auto triggerInput = inputs[TRIGGER_IN].getVoltage() > 0.1f;
  return triggerButton || triggerInput;
}

auto Upstage::waitIn() -> bool {
  auto waitButton = params[WAIT_BUTTON].getValue() > 0.1f;
  auto waitInput = inputs[WAIT_IN].getVoltage() > 0.1f;
  return waitButton || waitInput;
}

} // namespace dhe
