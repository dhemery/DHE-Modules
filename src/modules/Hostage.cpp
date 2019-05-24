#include "modules/Hostage.h"

#include "modules/controls/Duration.h"

namespace dhe {
Hostage::Hostage() :
    stateMachine{[this]() -> bool { return deferGateIsActive(); }, [this]() -> bool { return deferGateIn(); },
                 [this]() -> bool { return stageGateIn(); },       [this]() -> bool { return isSustainMode(); },
                 [this]() -> float { return duration(); },         [this](float) { forward(); },
                 [this](bool active) { setActive(active); },       [this](bool eoc) { setEoc(eoc); }} {
  config(ParameterCount, InputCount, OutputCount);

  duration::configKnob(this, DurationKnob, DurationRangeSwitch);
  duration::configSwitch(this, DurationRangeSwitch);
  duration = duration::withSelectableRange(this, DurationKnob, DurationCvInput, DurationRangeSwitch);

  configParam(ModeSwitch, 0.f, 1.f, 0.f, "Mode");

  stateMachine.start();
}

void Hostage::process(const ProcessArgs &args) { stateMachine.step(args.sampleTime); }

void Hostage::forward() { sendOut(envelopeIn()); }

auto Hostage::deferGateIsActive() const -> bool { return inputs[DeferGateInput].active; }

auto Hostage::isSustainMode() -> bool { return params[ModeSwitch].getValue() > 0.5f; }

void Hostage::setActive(bool active) {
  const auto voltage = active ? 10.f : 0.f;
  outputs[ActiveOutput].setVoltage(voltage);
}

void Hostage::setEoc(bool eoc) {
  const auto voltage = eoc ? 10.f : 0.f;
  outputs[EocOutput].setVoltage(voltage);
}

auto Hostage::deferGateIn() -> bool { return inputs[DeferGateInput].getVoltage() > 0.1f; }

auto Hostage::stageGateIn() -> bool { return inputs[GateInput].getVoltage() > 0.1f; }

auto Hostage::envelopeIn() -> float { return inputs[EnvelopeInput].getVoltage(); }

void Hostage::sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }
} // namespace dhe
