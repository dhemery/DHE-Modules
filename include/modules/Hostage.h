#pragma once

#include "Module.h"
#include "envelopes/HostageStateMachine.h"

#include <functional>

namespace dhe {

class Hostage : public Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override;

  auto deferGateIn() -> bool;
  auto deferGateIsActive() const -> bool;
  void forward();
  auto isSustainMode() -> bool;
  void setActive(bool active);
  void setEoc(bool eoc);
  auto stageGateIn() -> bool;

  enum ParameterIds { DurationKnob, DurationRangeSwitch, ModeSwitch, ParameterCount };

  enum InputIds { DeferGateInput, DurationCvInput, EnvelopeInput, GateInput, InputCount };

  enum OutputIds { ActiveOutput, EnvelopeOutput, EocOutput, OutputCount };

private:
  auto envelopeIn() -> float;
  void sendOut(float voltage);

  std::function<float()> duration;
  HostageStateMachine stateMachine;
};
} // namespace dhe
