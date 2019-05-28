#include "modules/Hostage.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

namespace dhe {
Hostage::Hostage() {
  config(ParameterCount, InputCount, OutputCount);

  duration::configKnob(this, DurationKnob, DurationRangeSwitch);
  duration::configSwitch(this, DurationRangeSwitch);

  toggle::config<2>(this, ModeSwitch, "Mode", {"Hold", "Sustain"}, 0);

  stateMachine.start();
}

void Hostage::process(const ProcessArgs &args) { stateMachine.step(args.sampleTime); }

void Hostage::forward() { sendOut(envelopeIn()); }

void Hostage::setActive(bool active) {
  auto const voltage = level::unipolarRange.scale(active);
  outputs[ActiveOutput].setVoltage(voltage);
}

void Hostage::setEoc(bool eoc) {
  auto const voltage = level::unipolarRange.scale(eoc);
  outputs[EocOutput].setVoltage(voltage);
}

auto Hostage::envelopeIn() -> float { return inputs[EnvelopeInput].getVoltage(); }

void Hostage::sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }
} // namespace dhe
