#include "modules/Stage.h"

#include "modules/controls/Curvature.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

namespace dhe {
Stage::Stage() :
    stateMachine{deferGateIsConnected,
                 isDeferring,
                 triggered,
                 duration,
                 [this](float /*unused*/) { forward(); },
                 [this]() { prepareToGenerate(); },
                 [this](float phase) { generate(phase); },
                 [this](bool active) { setActive(active); },
                 [this](bool eoc) { setEoc(eoc); }} {
  config(ParameterCount, InputCount, OutputCount);
  duration::configKnob(this, DurationKnob, duration::mediumRange);
  level::configKnob(this, LevelKnob, level::unipolarRange);
  curvature::configKnob(this, CurveKnob);

  stateMachine.start();
}

auto Stage::envelopeIn() -> float { return inputs[EnvelopeInput].getVoltage(); }

void Stage::forward() { sendOut(envelopeIn()); }

void Stage::generate(float phase) { sendOut(scale(taper(phase), startVoltage, level())); }

void Stage::prepareToGenerate() { startVoltage = envelopeIn(); }

void Stage::sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

void Stage::setActive(bool active) {
  const auto voltage = level::unipolarRange.scale(active);
  outputs[ActiveOutput].setVoltage(voltage);
}

void Stage::setEoc(bool eoc) {
  const auto voltage = level::unipolarRange.scale(eoc);
  outputs[EocOutput].setVoltage(voltage);
}

void Stage::process(const ProcessArgs &args) { stateMachine.step(args.sampleTime); }
} // namespace dhe
