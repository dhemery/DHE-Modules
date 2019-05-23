#pragma once

#include <functional>

#include "Module.h"

#include "envelopes/StageStateMachine.h"

namespace dhe {

class Stage : public Module {
public:
  Stage();
  void process(const ProcessArgs &args) override;

  auto deferGateIn() -> bool;
  auto deferGateIsActive() const -> bool;
  void forward();
  void generate(float phase);
  void prepareToGenerate();
  void setActive(bool active);
  void setEoc(bool eoc);
  auto stageGateIn() -> bool;

  enum ParameterIds { DurationKnob, LevelKnob, CurveKnob, ParameterCount };

  enum InputIds { EnvelopeIn, StageTriggerIn, DeferGateIn, InputCount };

  enum OutputIds { MainOut, EocOut, ActiveOut, OutputCount };

private:
  auto envelopeIn() -> float;
  void sendOut(float voltage);

  std::function<float()> duration;
  std::function<float()> level;
  std::function<float(float)> taper;
  StageStateMachine stateMachine;
  float startVoltage{0.f};
};
} // namespace dhe
