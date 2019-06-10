#pragma once

#include "modules/controls/DurationInputs.h"
#include "modules/envelopes/HostageMachine.h"

#include <engine/Module.hpp>

namespace dhe {

class Hostage : public HostageMachine, public rack::engine::Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override { HostageMachine::process(args.sampleTime); }

  enum ParameterIds { DurationKnob, DurationRangeSwitch, ModeSwitch, ParameterCount };

  enum InputIds { DeferInput, DurationCvInput, EnvelopeInput, TriggerInput, InputCount };

  enum OutputIds { ActiveOutput, EnvelopeOutput, EocOutput, OutputCount };

private:
  auto duration() const -> float override {
    return selectableDuration(this, DurationKnob, DurationCvInput, DurationRangeSwitch);
  }

  auto envelopeIn() const -> float override { return inputVoltage(this, EnvelopeInput); }

  auto deferIsHigh() const -> bool override { return inputIsHigh(this, DeferInput); }

  auto isSustainMode() const -> bool { return switchPosition(this, ModeSwitch) == 1; }

  void sendOut(float voltage) override { outputs[EnvelopeOutput].setVoltage(voltage); }

  void sendActive(bool active) override {
    auto const voltage = unipolarSignalRange.scale(active);
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void sendEoc(bool eoc) override {
    auto const voltage = unipolarSignalRange.scale(eoc);
    outputs[EocOutput].setVoltage(voltage);
  }

  auto stageGateIsHigh() const -> bool override { return inputIsHigh(this, TriggerInput); }
};
} // namespace dhe
