#include "modules/Hostage.h"
#include "modules/controls/Duration.h"
#include "modules/params/DurationParams.h"

namespace dhe {
Hostage::Hostage()
    : stateMachine{[this]() -> bool { return deferGateIsActive(); },
                   [this]() -> bool { return deferGateIn(); },
                   [this]() -> bool { return stageGateIn(); },
                   [this]() -> bool { return isSustainMode(); },
                   [this]() -> float { return duration(); },
                   [this](float) { forward(); },
                   [this](bool active) { setActive(active); },
                   [this](bool eoc) { setEoc(eoc); }} {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  duration::configKnob(this, DURATION_KNOB, DURATION_RANGE_SWITCH);
  duration::configSwitch(this, DURATION_RANGE_SWITCH);
  duration = duration::withSelectableRange(this, DURATION_KNOB, DURATION_CV,
                                           DURATION_RANGE_SWITCH);

  configParam(HOSTAGE_MODE_SWITCH, 0.f, 1.f, 0.f, "Mode");

  stateMachine.start();
}

void Hostage::process(const ProcessArgs &args) {
  stateMachine.step(args.sampleTime);
}

void Hostage::forward() { sendOut(envelopeIn()); }

auto Hostage::deferGateIsActive() const -> bool {
  return inputs[DEFER_GATE_IN].active;
}

auto Hostage::isSustainMode() -> bool {
  return params[HOSTAGE_MODE_SWITCH].getValue() > 0.5f;
}

void Hostage::setActive(bool active) {
  const auto voltage = active ? 10.f : 0.f;
  outputs[ACTIVE_OUT].setVoltage(voltage);
}

void Hostage::setEoc(bool eoc) {
  const auto voltage = eoc ? 10.f : 0.f;
  outputs[EOC_OUT].setVoltage(voltage);
}

auto Hostage::deferGateIn() -> bool {
  return inputs[DEFER_GATE_IN].getVoltage() > 0.1f;
}

auto Hostage::stageGateIn() -> bool {
  return inputs[STAGE_GATE_IN].getVoltage() > 0.1f;
}

auto Hostage::envelopeIn() -> float { return inputs[MAIN_IN].getVoltage(); }

void Hostage::sendOut(float voltage) { outputs[MAIN_OUT].setVoltage(voltage); }
} // namespace dhe
