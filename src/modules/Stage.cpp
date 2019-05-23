#include "modules/Stage.h"
#include "modules/controls/Curvature.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"
#include "modules/params/CurveParams.h"
#include "modules/params/DurationParams.h"
#include "modules/params/LevelParams.h"

namespace dhe {
Stage::Stage()
    : stateMachine{[this]() -> bool { return deferGateIsActive(); },
                   [this]() -> bool { return deferGateIn(); },
                   [this]() -> bool { return stageGateIn(); },
                   [this]() -> float { return duration(); },
                   [this](float) { forward(); },
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
  taper = curvature::withFixedShape(this, CurveKnob, &sigmoid::j_shape);

  stateMachine.start();
}

auto Stage::deferGateIn() -> bool {
  return inputs[DeferGateIn].getVoltage() > 0.1;
}

auto Stage::deferGateIsActive() const -> bool {
  return inputs[DeferGateIn].active;
}

auto Stage::envelopeIn() -> float { return inputs[EnvelopeIn].getVoltage(); }

void Stage::forward() { sendOut(envelopeIn()); }

void Stage::generate(float phase) {
  sendOut(scale(taper(phase), startVoltage, level()));
}

void Stage::prepareToGenerate() { startVoltage = envelopeIn(); }

void Stage::sendOut(float voltage) { outputs[MainOut].setVoltage(voltage); }

void Stage::setActive(bool active) {
  const auto voltage = active ? 10.f : 0.f;
  outputs[ActiveOut].setVoltage(voltage);
}

void Stage::setEoc(bool eoc) {
  const auto voltage = eoc ? 10.f : 0.f;
  outputs[EocOut].setVoltage(voltage);
}

auto Stage::stageGateIn() -> bool {
  return inputs[StageTriggerIn].getVoltage() > 0.1;
}

void Stage::process(const ProcessArgs &args) {
  stateMachine.step(args.sampleTime);
}
} // namespace dhe
