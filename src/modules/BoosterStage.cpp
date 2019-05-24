#include "modules/BoosterStage.h"

#include "modules/controls/Curvature.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

namespace dhe {

using ParamQuantity = rack::engine::ParamQuantity;

BoosterStage::BoosterStage() :
    stateMachine{[this]() -> bool { return deferGateIsActive(); },
                 [this]() -> bool { return deferGateIn(); },
                 [this]() -> bool { return stageGateIn(); },
                 [this]() -> float { return duration(); },
                 [this](float) { forward(); },
                 [this]() { prepareToGenerate(); },
                 [this](float phase) { generate(phase); },
                 [this](bool active) { setActive(active); },
                 [this](bool eoc) { setEoc(eoc); }} {
  config(ParameterCount, InputCount, OutputCount);

  duration::configKnob(this, DurationKnob, DurationRangeSwitch);
  duration::configSwitch(this, DurationRangeSwitch);
  duration = duration::withSelectableRange(this, DurationKnob, DurationCvInput, DurationRangeSwitch);

  level::configKnob(this, LevelKnob, LevelRangeSwitch);
  level::configSwitch(this, LevelRangeSwitch);
  level = level::withSelectableRange(this, LevelKnob, LevelCvInput, LevelRangeSwitch);

  curvature::configKnob(this, CurveKnob);
  curvature::configSwitch(this, ShapeSwitch);
  taper = curvature::withSelectableShape(this, CurveKnob, CurveCvInput, ShapeSwitch);

  configParam(ActiveButton, 0.f, 1.f, 0.f, "Active");
  configParam(DeferButton, 0.f, 1.f, 0.f, "Defer");
  configParam(EocButton, 0.f, 1.f, 0.f, "EOC");
  configParam(TriggerButton, 0.f, 1.f, 0.f, "TRIGGER");

  stateMachine.start();
}

void BoosterStage::process(const ProcessArgs &args) {
  stateMachine.step(args.sampleTime);

  sendActive();
  sendEoc();
}

void BoosterStage::forward() { sendOut(envelopeIn()); }

void BoosterStage::generate(float phase) { sendOut(scale(taper(phase), startVoltage, level())); }

void BoosterStage::setActive(bool active) { isActive = active; }

auto BoosterStage::stageGateIn() -> bool {
  auto const triggerButton = params[TriggerButton].getValue() > 0.5;
  auto const triggerInput = inputs[TriggerInput].getVoltage() > 0.1;
  return triggerButton || triggerInput;
}

void BoosterStage::prepareToGenerate() { startVoltage = envelopeIn(); }

auto BoosterStage::deferGateIsActive() const -> bool { return inputs[DeferGateInput].active; }

auto BoosterStage::deferGateIn() -> bool {
  auto const deferButton = params[DeferButton].getValue() > 0.5f;
  auto const deferInput = inputs[DeferGateInput].getVoltage() > 0.1f;
  return deferButton || deferInput;
}

void BoosterStage::setEoc(bool eoc) { isEoc = eoc; }

auto BoosterStage::envelopeIn() -> float { return inputs[EnvelopeInput].getVoltage(); }

void BoosterStage::sendActive() {
  auto const activeButton = params[ActiveButton].getValue() > 0.5f;
  auto const voltage = isActive || activeButton ? 10.f : 0.f;
  outputs[ActiveOutput].setVoltage(voltage);
}

void BoosterStage::sendEoc() {
  auto const eocButton = params[EocButton].getValue() > 0.5f;
  auto const voltage = isEoc || eocButton ? 10.f : 0.f;
  outputs[EocOutput].setVoltage(voltage);
}

void BoosterStage::sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }
} // namespace dhe
