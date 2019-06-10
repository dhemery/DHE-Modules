#pragma once

#include "envelopes/StageStateMachine.h"
#include "modules/controls/CurvatureInputs.h"
#include "modules/controls/DurationInputs.h"
#include "modules/controls/LevelInputs.h"
#include "modules/envelopes/StageMachine.h"

#include <engine/Module.hpp>

namespace dhe {

class Stage : public StageMachine, public rack::engine::Module {
public:
  Stage();
  void process(const ProcessArgs &args) override { StageMachine::process(args.sampleTime); }

  enum ParameterIds { DurationKnob, LevelKnob, CurveKnob, ParameterCount };

  enum InputIds { EnvelopeInput, TriggerInput, DeferInput, InputCount };

  enum OutputIds { EnvelopeOutput, EocOutput, ActiveOutput, OutputCount };

private:
  auto deferIsHigh() const -> bool override { return inputIsHigh(this, DeferInput); }

  auto duration() const -> float override { return dhe::duration(this, DurationKnob, mediumDurationRange); }

  auto envelopeIn() const -> float override { return inputVoltage(this, EnvelopeInput); }

  auto level() const -> float override { return dhe::level(this, LevelKnob, unipolarSignalRange); }

  void sendActive(bool active) override {
    const auto voltage = unipolarSignalRange.scale(active);
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void sendEoc(bool eoc) override {
    const auto voltage = unipolarSignalRange.scale(eoc);
    outputs[EocOutput].setVoltage(voltage);
  }

  void sendOut(float voltage) override { outputs[EnvelopeOutput].setVoltage(voltage); }

  auto taper(float input) const -> float override {
    return taper::variableJTaper.apply(input, curvature(this, CurveKnob));
  }

  auto triggerIsHigh() const -> bool override { return inputIsHigh(this, TriggerInput); }
};
} // namespace dhe
