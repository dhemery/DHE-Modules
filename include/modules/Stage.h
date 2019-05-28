#pragma once

#include "Module.h"
#include "envelopes/StageStateMachine.h"
#include "modules/controls/Curvature.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

#include <functional>

namespace dhe {

class Stage : public Module {
public:
  Stage();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { DurationKnob, LevelKnob, CurveKnob, ParameterCount };

  enum InputIds { EnvelopeInput, TriggerInput, DeferInput, InputCount };

  enum OutputIds { EnvelopeOutput, EocOutput, ActiveOutput, OutputCount };

private:
  auto envelopeIn() -> float;
  void forward();
  void generate(float phase);
  void prepareToGenerate();
  void sendOut(float voltage);
  void setActive(bool active);
  void setEoc(bool eoc);

  float startVoltage{0.F};

  std::function<float()> const level{level::withUnipolarRange(this, LevelKnob)};
  std::function<float(float)> const taper{taper::jShaped(this, CurveKnob)};

  StageStateMachine stateMachine{
      input::isConnected(this, DeferInput),     input::isHigh(this, DeferInput),
      input::isHigh(this, TriggerInput),        duration::withMediumRange(this, DurationKnob),
      [this](float /*unused*/) { forward(); },  [this]() { prepareToGenerate(); },
      [this](float phase) { generate(phase); }, [this](bool active) { setActive(active); },
      [this](bool eoc) { setEoc(eoc); }};
};
} // namespace dhe
