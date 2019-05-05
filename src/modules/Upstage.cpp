#include "modules/Upstage.h"

namespace DHE {

Upstage::Upstage() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configParam(LEVEL_KNOB, 0.f, 1.f, 0.5f, "Level", "%", 0.f, 100.f, 0.f);
  configSignalRange(LEVEL_RANGE_SWITCH, "Level", true);

  configParam(TRIGGER_BUTTON, 0.f, 1.f, 0.f, "Trigger");
  configParam(WAIT_BUTTON, 0.f, 1.f, 0.f, "Wait");

  level.config(&params[LEVEL_KNOB], &params[LEVEL_RANGE_SWITCH]);
}

void Upstage::process(const ProcessArgs &args) {
  auto is_triggered = trigger_in() && !wait_in();
  send_trigger(is_triggered);
  send_envelope(level());
}

void Upstage::send_envelope(float voltage) {
  outputs[MAIN_OUT].value = voltage;
}

void Upstage::send_trigger(bool is_triggered) {
  outputs[TRIGGER_OUT].value = is_triggered ? 10.f : 0.f;
}

auto Upstage::trigger_in() const -> bool {
  auto trigger_button = params[TRIGGER_BUTTON].value > 0.1f;
  auto trigger_input = inputs[TRIGGER_IN].value > 0.1f;
  return trigger_button || trigger_input;
}

auto Upstage::wait_in() const -> bool {
  auto wait_button = params[WAIT_BUTTON].value > 0.1f;
  auto wait_input = inputs[WAIT_IN].value > 0.1f;
  return wait_button || wait_input;
}

} // namespace DHE
