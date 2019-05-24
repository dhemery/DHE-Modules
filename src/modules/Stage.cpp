#include "modules/Stage.h"

#include "modules/controls/Curvature.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

namespace dhe {
Stage::Stage() :
    stateMachine{[this]() -> bool { return deferGateIsActive(); },
                 [this]() -> bool { return deferGateIn(); },
                 [this]() -> bool { return stageGateIn(); },
                 [this]() -> float { return duration(); },
                 [this](float /*unused*/) { forward(); },
                 [this]() { prepareToGenerate(); },
                 [this](float phase) { generate(phase); },
                 [this](bool active) { setActive(active); },
                 [this](bool eoc) { setEoc(eoc); }} {
  config(ParameterCount, InputCount, OutputCount);

  auto const durationRange = duration::mediumRange;
  duration::configKnob(this, DurationKnob, durationRange);
  duration = duration::withFixedRange(this, DurationKnob, durationRange);

  auto const levelRange = level::unipolarRange;
  level::configKnob(this, LevelKnob, levelRange);
  level = level::withFixedRange(this, LevelKnob, levelRange);

  curvature::configKnob(this, CurveKnob);
  taper = curvature::withFixedShape(this, CurveKnob, &sigmoid::jShape);

  stateMachine.start();
}

auto Stage::deferGateIn() -> bool { return inputs[DeferGateInput].getVoltage() > 0.1; }

auto Stage::deferGateIsActive() const -> bool { return inputs[DeferGateInput].active; }

auto Stage::envelopeIn() -> float { return inputs[EnvelopeInput].getVoltage(); }

void Stage::forward() { sendOut(envelopeIn()); }

void Stage::generate(float phase) { sendOut(scale(taper(phase), startVoltage, level())); }

void Stage::prepareToGenerate() { startVoltage = envelopeIn(); }

void Stage::sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

void Stage::setActive(bool active) {
  const auto voltage = active ? 10.F : 0.F;
  outputs[ActiveOutput].setVoltage(voltage);
}

void Stage::setEoc(bool eoc) {
  const auto voltage = eoc ? 10.F : 0.F;
  outputs[EocOutput].setVoltage(voltage);
}

auto Stage::stageGateIn() -> bool { return inputs[TriggerInput].getVoltage() > 0.1; }

void Stage::process(const ProcessArgs &args) { stateMachine.step(args.sampleTime); }
} // namespace dhe
