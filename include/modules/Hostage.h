#pragma once

#include "envelopes/HostageStateMachine.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/Functions.h"

#include <engine/Module.hpp>
#include <modules/controls/DurationInputs.h>

namespace dhe {

class Hostage : public rack::engine::Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { DurationKnob, DurationRangeSwitch, ModeSwitch, ParameterCount };

  enum InputIds { DeferInput, DurationCvInput, EnvelopeInput, TriggerInput, InputCount };

  enum OutputIds { ActiveOutput, EnvelopeOutput, EocOutput, OutputCount };

private:
  auto duration() const -> float {
    return selectableDuration(this, DurationKnob, DurationCvInput, DurationRangeSwitch);
  }

  auto envelopeIn() const -> float { return paramValue(this, EnvelopeInput); }

  void forward() { sendOut(envelopeIn()); }

  void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void setActive(bool active) {
    auto const voltage = unipolarSignalRange.scale(active);
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void setEoc(bool eoc) {
    auto const voltage = unipolarSignalRange.scale(eoc);
    outputs[EocOutput].setVoltage(voltage);
  }

  HostageStateMachine stateMachine{
      inputIsConnectedFunction(this, DeferInput), inputIsHighFunction(this, DeferInput),
      inputIsHighFunction(this, TriggerInput),    buttonIsPressedFunction(this, ModeSwitch),
      [this]() -> float { return duration(); },   [this](float /*unused*/) { forward(); },
      [this](bool active) { setActive(active); }, [this](bool eoc) { setEoc(eoc); }};
};
} // namespace dhe
