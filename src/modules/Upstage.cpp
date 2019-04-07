#include <modules/Upstage.h>

#include "util/rotation.h"

namespace DHE {

Upstage::Upstage() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

void Upstage::step() {
  auto is_triggered = trigger_in() && !wait_in();
  send_trigger(is_triggered);
  send_envelope(envelope_voltage());
}

auto Upstage::envelope_voltage() const -> float {
  return level_range->scale(level());
}

auto Upstage::level() const -> float { return modulated(LEVEL_KNOB, LEVEL_CV); }

auto Upstage::modulated(Upstage::ParameterIds knob_param,
                        Upstage::InputIds cv_input) const -> float {
  auto rotation = params[knob_param].value;
  auto cv = inputs[cv_input].value;
  return Rotation::modulated(rotation, cv);
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
