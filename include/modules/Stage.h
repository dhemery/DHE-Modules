#pragma once

#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"
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
  auto deferIsHigh() const -> bool override { return inputIsHigh(inputs[DeferInput]); }

  auto duration() const -> float override { return dhe::duration(params[DurationKnob], mediumDurationRange); }

  auto envelopeIn() const -> float override { return inputVoltage(inputs[EnvelopeInput]); }

  auto level() const -> float override { return dhe::level(params[LevelKnob], unipolarSignalRange); }

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
    return taper::variableJTaper.apply(input, curvature(params[CurveKnob]));
  }

  auto triggerIsHigh() const -> bool override { return inputIsHigh(inputs[TriggerInput]); }
};
} // namespace dhe
