#pragma once

#include "Module.h"
#include "envelopes/StageStateMachine.h"
#include "modules/controls/Controls.h"
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

  std::function<bool()> deferGateIsConnected{input::isConnected(this, DeferInput)};
  std::function<bool()> isDeferring{input::isHigh(this, DeferInput)};
  std::function<float()> duration{duration::withMediumRange(this, DurationKnob)};
  std::function<float()> level{level::withUnipolarRange(this, LevelKnob)};
  std::function<float(float)> taper{taper::jShaped(this, CurveKnob)};
  std::function<bool()> triggered{input::isHigh(this, TriggerInput)};

  StageStateMachine stateMachine;
};
} // namespace dhe
