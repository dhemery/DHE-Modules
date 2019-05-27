#pragma once

#include "Module.h"
#include "envelopes/HostageStateMachine.h"

#include <functional>

namespace dhe {

class Hostage : public Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { DurationKnob, DurationRangeSwitch, ModeSwitch, ParameterCount };

  enum InputIds { DeferGateInput, DurationCvInput, EnvelopeInput, GateInput, InputCount };

  enum OutputIds { ActiveOutput, EnvelopeOutput, EocOutput, OutputCount };

private:
  auto deferGateIn() -> bool;
  auto deferGateIsActive() const -> bool;
  auto envelopeIn() -> float;
  void forward();
  auto isSustainMode() -> bool;
  void sendOut(float voltage);
  void setActive(bool active);
  void setEoc(bool eoc);
  auto stageGateIn() -> bool;

  std::function<float()> duration;
  HostageStateMachine stateMachine;
};
} // namespace dhe
