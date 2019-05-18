#include <utility>

#include "modules/BoosterStage.h"

#include "util/gain.h"
#include "util/signal.h"

namespace DHE {

BoosterStage::BoosterStage()
    : state_machine{[this]() -> bool { return defer_gate_is_active(); },
                    [this]() -> bool { return defer_gate_in(); },
                    [this]() -> bool { return stage_gate_in(); },
                    [this]() -> float { return duration->seconds(); },
                    [this](float) { forward(); },
                    [this]() { prepare_to_generate(); },
                    [this](float phase) { generate(phase); },
                    [this](bool active) { set_active(active); },
                    [this](bool eoc) { set_eoc(eoc); }} {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configParam(DURATION_KNOB, 0.f, 1.f, 0.5f, "Duration");
  configParam(DURATION_RANGE_SWITCH, 0.f, 2.f, 1.f, "Max Duration");
  configParam(LEVEL_KNOB, 0.f, 1.f, 0.5f, "Level", "%", 0.f, 100.f, 0.f);
  configParam(CURVE_KNOB, 0.f, 1.f, 0.5f, "Curvature", "%", 0.f, 100.f, 0.f);

  configSignalRange(LEVEL_RANGE_SWITCH, "Level", true);
  configParam(SHAPE_SWITCH, 0.f, 1.f, 0.f, "Curve Shape");

  configParam(ACTIVE_BUTTON, 0.f, 1.f, 0.f, "Active");
  configParam(DEFER_BUTTON, 0.f, 1.f, 0.f, "Defer");
  configParam(EOC_BUTTON, 0.f, 1.f, 0.f, "EOC");
  configParam(TRIGGER_BUTTON, 0.f, 1.f, 0.f, "TRIGGER");

  duration = std::unique_ptr<DurationControl>(
      new DurationControl(params[DURATION_KNOB], params[DURATION_RANGE_SWITCH],
                          inputs[DURATION_CV]));
  level = std::unique_ptr<LevelControl>(new LevelControl(
      params[LEVEL_KNOB], params[LEVEL_RANGE_SWITCH], inputs[LEVEL_CV]));

  state_machine.start();
}

void BoosterStage::process(const ProcessArgs &args) {
  set_shape();

  state_machine.step(args.sampleTime);
}

void BoosterStage::forward() { send_out(envelope_in()); }

void BoosterStage::generate(float phase) {
  send_out(scale(taper(phase), start_voltage, level->voltage()));
}

void BoosterStage::set_active(bool active) {
  is_active = active;
  send_active();
}

auto BoosterStage::stage_gate_in() -> bool {
  auto trigger_button = params[TRIGGER_BUTTON].getValue() > 0.5;
  auto trigger_input = inputs[STAGE_TRIGGER_IN].getVoltage() > 0.1;
  return trigger_button || trigger_input;
}

void BoosterStage::prepare_to_generate() { start_voltage = envelope_in(); }

auto BoosterStage::defer_gate_is_active() const -> bool {
  return inputs[DEFER_GATE_IN].active;
}

auto BoosterStage::defer_gate_in() -> bool {
  auto defer_button = params[DEFER_BUTTON].getValue() > 0.5f;
  auto defer_input = inputs[DEFER_GATE_IN].getVoltage() > 0.1f;
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

auto BoosterStage::envelope_in() -> float {
  return inputs[ENVELOPE_IN].getVoltage();
}

auto BoosterStage::curvature() -> float {
  return Sigmoid::curvature(modulated(CURVE_KNOB, CURVE_CV));
}

void BoosterStage::send_active() {
  auto const voltage = is_active || active_button_is_pressed ? 10.f : 0.f;
  outputs[ACTIVE_OUT].setVoltage(voltage);
}

void BoosterStage::send_eoc() {
  const auto voltage = is_eoc || eoc_button_is_pressed ? 10.f : 0.f;
  outputs[EOC_OUT].setVoltage(voltage);
}

void BoosterStage::send_out(float voltage) {
  outputs[MAIN_OUT].setVoltage(voltage);
}

auto BoosterStage::taper(float phase) -> float {
  return curve_shape->taper(phase, curvature());
}

void BoosterStage::set_shape() {
  const auto choice = static_cast<int>(params[SHAPE_SWITCH].getValue());
  curve_shape = Sigmoid::shapes()[choice];
}

} // namespace DHE
