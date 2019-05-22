#include "modules/Upstage.h"
#include "modules/controls/Level.h"
#include "modules/params/LevelParams.h"

namespace dhe {

Upstage::Upstage() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  level::configKnob(this, LEVEL_KNOB, LEVEL_RANGE_SWITCH);
  level::configSwitch(this, LEVEL_RANGE_SWITCH);
  level =
      level::withCvAndSwitch(this, LEVEL_KNOB, LEVEL_CV, LEVEL_RANGE_SWITCH);

  configParam(TRIGGER_BUTTON, 0.f, 1.f, 0.f, "Trigger");
  configParam(WAIT_BUTTON, 0.f, 1.f, 0.f, "Wait");
}

void Upstage::process(const ProcessArgs &args) {
  auto is_triggered = trigger_in() && !wait_in();
  send_trigger(is_triggered);
  send_envelope(level());
}

void Upstage::send_envelope(float voltage) {
  outputs[MAIN_OUT].setVoltage(voltage);
}

void Upstage::send_trigger(bool is_triggered) {
  const auto voltage = is_triggered ? 10.f : 0.f;
  outputs[TRIGGER_OUT].setVoltage(voltage);
}

auto Upstage::trigger_in() -> bool {
  auto trigger_button = params[TRIGGER_BUTTON].getValue() > 0.1f;
  auto trigger_input = inputs[TRIGGER_IN].getVoltage() > 0.1f;
  return trigger_button || trigger_input;
}

auto Upstage::wait_in() -> bool {
  auto wait_button = params[WAIT_BUTTON].getValue() > 0.1f;
  auto wait_input = inputs[WAIT_IN].getVoltage() > 0.1f;
  return wait_button || wait_input;
}

} // namespace dhe
