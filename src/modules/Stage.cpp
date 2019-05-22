#include "modules/Stage.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"
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
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configParam(CURVE_KNOB, 0.f, 1.f, 0.5f, "Curvature", "%", 0.f, 200.f, -100.f);

  auto const durationRange = duration::mediumRange;
  duration::configKnob(this, DURATION_KNOB, durationRange);
  duration = duration::withFixedRange(this, DURATION_KNOB, durationRange);

  auto const levelRange = level::unipolarRange;
  level::configKnob(this, LEVEL_KNOB, levelRange);
  level = level::withFixedRange(this, LEVEL_KNOB, levelRange);

  shape = std::unique_ptr<CurvatureControl>(
      new CurvatureControl(params[CURVE_KNOB]));

  stateMachine.start();
}

auto Stage::deferGateIn() -> bool {
  return inputs[DEFER_GATE_IN].getVoltage() > 0.1;
}

auto Stage::deferGateIsActive() const -> bool {
  return inputs[DEFER_GATE_IN].active;
}

auto Stage::envelopeIn() -> float { return inputs[ENVELOPE_IN].getVoltage(); }

void Stage::forward() { sendOut(envelopeIn()); }

void Stage::generate(float phase) {
  auto const taperedPhase = shape->taper(phase);
  sendOut(scale(taperedPhase, startVoltage, level()));
}

void Stage::prepareToGenerate() { startVoltage = envelopeIn(); }

void Stage::sendOut(float voltage) { outputs[MAIN_OUT].setVoltage(voltage); }

void Stage::setActive(bool active) {
  const auto voltage = active ? 10.f : 0.f;
  outputs[ACTIVE_OUT].setVoltage(voltage);
}

void Stage::setEoc(bool eoc) {
  const auto voltage = eoc ? 10.f : 0.f;
  outputs[EOC_OUT].setVoltage(voltage);
}

auto Stage::stageGateIn() -> bool {
  return inputs[STAGE_TRIGGER_IN].getVoltage() > 0.1;
}

void Stage::process(const ProcessArgs &args) {
  stateMachine.step(args.sampleTime);
}
} // namespace dhe
