#include <utility>

#include "modules/BoosterStage.h"

#include "util/gain.h"
#include "util/signal.h"

namespace DHE {

BoosterStage::BoosterStage()
    : stateMachine{[this]() -> bool { return deferGateIsActive(); },
                   [this]() -> bool { return deferGateIn(); },
                   [this]() -> bool { return stageGateIn(); },
                   [this]() -> float { return duration->seconds(); },
                   [this](float) { forward(); },
                   [this]() { prepareToGenerate(); },
                   [this](float phase) { generate(phase); },
                   [this](bool active) { setActive(active); },
                   [this](bool eoc) { setEoc(eoc); }} {
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

  shape = std::unique_ptr<CurvatureControl>(new CurvatureControl(
      params[CURVE_KNOB], params[SHAPE_SWITCH], inputs[CURVE_CV]));

  stateMachine.start();
}

void BoosterStage::process(const ProcessArgs &args) {
  stateMachine.step(args.sampleTime);

  sendActive();
  sendEoc();
}

void BoosterStage::forward() { sendOut(envelopeIn()); }

void BoosterStage::generate(float phase) {
  auto const taperedPhase = shape->taper(phase);
  sendOut(scale(taperedPhase, startVoltage, level->voltage()));
}

void BoosterStage::setActive(bool active) { isActive = active; }

auto BoosterStage::stageGateIn() -> bool {
  auto const triggerButton = params[TRIGGER_BUTTON].getValue() > 0.5;
  auto const triggerInput = inputs[STAGE_TRIGGER_IN].getVoltage() > 0.1;
  return triggerButton || triggerInput;
}

void BoosterStage::prepareToGenerate() { startVoltage = envelopeIn(); }

auto BoosterStage::deferGateIsActive() const -> bool {
  return inputs[DEFER_GATE_IN].active;
}

auto BoosterStage::deferGateIn() -> bool {
  auto const deferButton = params[DEFER_BUTTON].getValue() > 0.5f;
  auto const deferInput = inputs[DEFER_GATE_IN].getVoltage() > 0.1f;
  return deferButton || deferInput;
}

void BoosterStage::setEoc(bool eoc) { isEoc = eoc; }

auto BoosterStage::envelopeIn() -> float {
  return inputs[ENVELOPE_IN].getVoltage();
}

void BoosterStage::sendActive() {
  auto const activeButton = params[ACTIVE_BUTTON].getValue() > 0.5f;
  auto const voltage = isActive || activeButton ? 10.f : 0.f;
  outputs[ACTIVE_OUT].setVoltage(voltage);
}

void BoosterStage::sendEoc() {
  auto const eocButton = params[EOC_BUTTON].getValue() > 0.5f;
  auto const voltage = isEoc || eocButton ? 10.f : 0.f;
  outputs[EOC_OUT].setVoltage(voltage);
}

void BoosterStage::sendOut(float voltage) {
  outputs[MAIN_OUT].setVoltage(voltage);
}
} // namespace DHE
