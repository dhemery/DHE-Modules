#include <utility>

#include "modules/BoosterStage.h"

#include "util/duration.h"
#include "util/gain.h"
#include "util/signal.h"

namespace DHE {

BoosterStage::BoosterStage(const std::function<float()> &sample_time)
    : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
      state_machine{[this]() -> bool { return defer_gate_is_active(); },
                    [this]() -> bool { return defer_gate_in(); },
                    [this]() -> bool { return stage_gate_in(); },
                    [this]() -> float { return duration(); },
                    sample_time,
                    [this]() { forward(); },
                    [this]() { prepare_to_generate(); },
                    [this](float phase) { generate(phase); },
                    [this](bool active) { set_active(active); },
                    [this](bool eoc) { set_eoc(eoc); }} {
  state_machine.start();
}

void BoosterStage::step() { state_machine.step(); }

void BoosterStage::forward() { send_out(envelope_in()); }

void BoosterStage::generate(float phase) {
  send_out(scale(taper(phase), start_voltage, level()));
}

void BoosterStage::set_active(bool active) {
  is_active = active;
  send_active();
}

auto BoosterStage::stage_gate_in() const -> bool {
  auto trigger_button = params[TRIGGER_BUTTON].value > 0.5;
  auto trigger_input = inputs[STAGE_TRIGGER_IN].value > 0.1;
  return trigger_button || trigger_input;
}

void BoosterStage::prepare_to_generate() { start_voltage = envelope_in(); }

auto BoosterStage::defer_gate_is_active() const -> bool {
  return inputs[DEFER_GATE_IN].active;
}

auto BoosterStage::duration() const -> float {
  auto rotation = modulated(DURATION_KNOB, DURATION_CV);
  return DHE::duration(rotation, *duration_range);
}

auto BoosterStage::defer_gate_in() const -> bool {
  auto defer_button = params[DEFER_BUTTON].value > 0.5f;
  auto defer_input = inputs[DEFER_GATE_IN].value > 0.1f;
  return defer_button || defer_input;
}

void BoosterStage::set_active_button(bool active) {
  active_button_is_pressed = active;
  send_active();
}

void BoosterStage::set_eoc(bool eoc) {
  is_eoc = eoc;
  send_eoc();
}

void BoosterStage::set_eoc_button(bool eoc) {
  eoc_button_is_pressed = eoc;
  send_eoc();
}

auto BoosterStage::envelope_in() const -> float {
  return inputs[ENVELOPE_IN].value;
}

auto BoosterStage::curvature() const -> float {
  return Sigmoid::curvature(modulated(CURVE_KNOB, CURVE_CV));
}

auto BoosterStage::is_s_shape() const -> bool {
  return params[SHAPE_SWITCH].value > 0.5f;
}

auto BoosterStage::level() const -> float {
  auto level = modulated(LEVEL_KNOB, LEVEL_CV);
  return level_range->scale(level);
}

void BoosterStage::send_active() {
  outputs[ACTIVE_OUT].value =
      is_active || active_button_is_pressed ? 10.f : 0.f;
}

void BoosterStage::send_eoc() {
  outputs[EOC_OUT].value = is_eoc || eoc_button_is_pressed ? 10.f : 0.f;
}

void BoosterStage::send_out(float voltage) {
  outputs[MAIN_OUT].value = voltage;
}

auto BoosterStage::taper(float phase) const -> float {
  return Sigmoid::taper(phase, curvature(), is_s_shape());
}

} // namespace DHE
