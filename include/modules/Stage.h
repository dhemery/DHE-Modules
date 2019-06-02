#pragma once

#include "envelopes/StageStateMachine.h"
#include "modules/controls/Controls.h"
#include "modules/controls/CurvatureControls.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

#include <engine/Module.hpp>

namespace dhe {

class Stage : public rack::engine::Module {
public:
  Stage();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { DurationKnob, LevelKnob, CurveKnob, ParameterCount };

  enum InputIds { EnvelopeInput, TriggerInput, DeferInput, InputCount };

  enum OutputIds { EnvelopeOutput, EocOutput, ActiveOutput, OutputCount };

private:
  auto envelopeIn() const -> float { return inputVoltage(this, EnvelopeInput); }

  void forward() { sendOut(envelopeIn()); }

  void generate(float phase) { sendOut(scale(taper(phase), startVoltage, level())); }

  auto level() const -> float { return scaledRotation(this, LevelKnob, level::unipolarRange); }

  void prepareToGenerate() { startVoltage = envelopeIn(); }

  void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void setActive(bool active) {
    const auto voltage = level::unipolarRange.scale(active);
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void setEoc(bool eoc) {
    const auto voltage = level::unipolarRange.scale(eoc);
    outputs[EocOutput].setVoltage(voltage);
  }

  auto taper(float input) const -> float { return taper::variableJTaper.apply(input, curvature(this, CurveKnob)); }

  float startVoltage{0.F};
  StageStateMachine stateMachine{
      inputIsConnectedFunction(this, DeferInput), inputIsHighFunction(this, DeferInput),
      inputIsHighFunction(this, TriggerInput),    duration::withMediumRange(this, DurationKnob),
      [this](float /*unused*/) { forward(); },    [this]() { prepareToGenerate(); },
      [this](float phase) { generate(phase); },   [this](bool active) { setActive(active); },
      [this](bool eoc) { setEoc(eoc); }};
};
} // namespace dhe
