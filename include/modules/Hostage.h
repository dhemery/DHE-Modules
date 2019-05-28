#pragma once

#include "Module.h"
#include "envelopes/HostageStateMachine.h"
#include "modules/controls/Controls.h"
#include "modules/controls/Duration.h"

#include <functional>

namespace dhe {

class Hostage : public Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { DurationKnob, DurationRangeSwitch, ModeSwitch, ParameterCount };

  enum InputIds { DeferInput, DurationCvInput, EnvelopeInput, TriggerInput, InputCount };

  enum OutputIds { ActiveOutput, EnvelopeOutput, EocOutput, OutputCount };

private:
  auto envelopeIn() -> float;
  void forward();
  void sendOut(float voltage);
  void setActive(bool active);
  void setEoc(bool eoc);

  HostageStateMachine stateMachine{
      input::isConnected(this, DeferInput),
      input::isHigh(this, DeferInput),
      input::isHigh(this, TriggerInput),
      button::state(this, ModeSwitch),
      duration::withSelectableRange(this, DurationKnob, DurationCvInput, DurationRangeSwitch),
      [this](float /*unused*/) { forward(); },
      [this](bool active) { setActive(active); },
      [this](bool eoc) { setEoc(eoc); }};
};
} // namespace dhe
