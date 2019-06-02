#pragma once

#include "envelopes/HostageStateMachine.h"
#include "modules/controls/Functions.h"
#include "modules/controls/DurationConfig.h"

#include <engine/Module.hpp>

namespace dhe {

class Hostage : public rack::engine::Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { DurationKnob, DurationRangeSwitch, ModeSwitch, ParameterCount };

  enum InputIds { DeferInput, DurationCvInput, EnvelopeInput, TriggerInput, InputCount };

  enum OutputIds { ActiveOutput, EnvelopeOutput, EocOutput, OutputCount };

private:
  void forward() { sendOut(envelopeIn()); }

  void setActive(bool active) {
    auto const voltage = unipolarSignalRange.scale(active);
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void setEoc(bool eoc) {
    auto const voltage = unipolarSignalRange.scale(eoc);
    outputs[EocOutput].setVoltage(voltage);
  }

  auto envelopeIn() const -> float { return paramValue(this, EnvelopeInput); }

  void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  HostageStateMachine stateMachine{
      inputIsConnectedFunction(this, DeferInput),
      inputIsHighFunction(this, DeferInput),
      inputIsHighFunction(this, TriggerInput),
      buttonIsPressedFunction(this, ModeSwitch),
      duration::withSelectableRange(this, DurationKnob, DurationCvInput, DurationRangeSwitch),
      [this](float /*unused*/) { forward(); },
      [this](bool active) { setActive(active); },
      [this](bool eoc) { setEoc(eoc); }};
};
} // namespace dhe
