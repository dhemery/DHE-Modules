#include "modules/BoosterStage.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Curvature.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

#include <array>
#include <string>

namespace dhe {

BoosterStage::BoosterStage() :
    stateMachine{deferIsConnected,
                 [this]() -> bool { return deferButton() || deferInput(); },
                 [this]() -> bool { return triggerButton() || triggerInput(); },
                 duration,
                 [this](float /*unused*/) { forward(); },
                 [this]() { prepareToGenerate(); },
                 [this](float phase) { generate(phase); },
                 [this](bool active) { setActive(active); },
                 [this](bool eoc) { setEoc(eoc); }} {
  config(ParameterCount, InputCount, OutputCount);

  duration::configKnob(this, DurationKnob, DurationRangeSwitch);
  duration::configSwitch(this, DurationRangeSwitch);

  level::configKnob(this, LevelKnob, LevelRangeSwitch);
  level::configSwitch(this, LevelRangeSwitch);

  curvature::configKnob(this, CurveKnob);
  curvature::configSwitch(this, ShapeSwitch);

  button::config(this, DeferButton, "DEFER", {"From input", "High"}, 0);
  button::config(this, TriggerButton, "TRIG", {"From input", "High"}, 0);
  button::config(this, ActiveButton, "ACTIVE", {"Generated", "High"}, 0);
  button::config(this, EocButton, "EOC", {"Generated", "High"}, 0);

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

void BoosterStage::prepareToGenerate() { startVoltage = envelopeIn(); }

auto BoosterStage::deferGateIsActive() const -> bool { return inputs[DeferInput].active; }

void BoosterStage::setEoc(bool eoc) { isEoc = eoc; }

auto BoosterStage::envelopeIn() -> float { return inputs[EnvelopeInput].getVoltage(); }

void BoosterStage::sendActive() {
  auto const voltage = level::unipolarRange.scale(isActive || activeButton());
  outputs[ActiveOutput].setVoltage(voltage);
}

void BoosterStage::sendEoc() {
  auto const voltage = level::unipolarRange.scale(isEoc || eocButton());
  outputs[EocOutput].setVoltage(voltage);
}

void BoosterStage::sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }
} // namespace dhe
