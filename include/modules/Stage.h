#pragma once

#include "envelopes/StageStateMachine.h"
#include "modules/controls/CurvatureInputs.h"
#include "modules/controls/DurationInputs.h"
#include "modules/controls/Functions.h"

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
  auto duration() const -> float { return dhe::duration(this, DurationKnob, mediumDurationRange); }

  auto envelopeIn() const -> float { return inputVoltage(this, EnvelopeInput); }

  void forward() { sendOut(envelopeIn()); }

  void generate(float phase) { sendOut(scale(taper(phase), startVoltage, level())); }

  auto level() const -> float { return scaledRotation(this, LevelKnob, unipolarSignalRange); }

  void prepareToGenerate() { startVoltage = envelopeIn(); }

  void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void setActive(bool active) {
    const auto voltage = unipolarSignalRange.scale(active);
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void setEoc(bool eoc) {
    const auto voltage = unipolarSignalRange.scale(eoc);
    outputs[EocOutput].setVoltage(voltage);
  }

  auto taper(float input) const -> float { return taper::variableJTaper.apply(input, curvature(this, CurveKnob)); }

  auto deferIsConnected() const -> bool { return inputIsConnected(this, DeferInput); }
  auto isDeferring() const -> bool { return inputIsHigh(this, DeferInput); }
  auto isTriggered() const -> bool { return inputIsHigh(this, TriggerInput); }

  float startVoltage{0.F};
  StageStateMachine stateMachine{[this]() -> bool { return deferIsConnected(); },
                                 [this]() -> bool { return isDeferring(); },
                                 [this]() -> bool { return isTriggered(); },
                                 [this]() -> float { return duration(); },
                                 [this](float /*unused*/) { forward(); },
                                 [this]() { prepareToGenerate(); },
                                 [this](float phase) { generate(phase); },
                                 [this](bool active) { setActive(active); },
                                 [this](bool eoc) { setEoc(eoc); }};
};
} // namespace dhe
