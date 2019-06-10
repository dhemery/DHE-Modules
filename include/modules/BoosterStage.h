#pragma once

#include "modules/components/Taper.h"
#include "modules/controls/CommonInputs.h"
#include "modules/controls/CurvatureInputs.h"
#include "modules/controls/DurationInputs.h"
#include "modules/controls/LevelInputs.h"
#include "modules/envelopes/StageMachine.h"

#include <engine/Module.hpp>

namespace dhe {

class BoosterStage : public StageMachine, public rack::engine::Module {
public:
  BoosterStage();
  void process(const ProcessArgs &args) override { StageMachine::process(args.sampleTime); }

  enum ParameterIds {
    ActiveButton,
    CurveKnob,
    DeferButton,
    DurationKnob,
    DurationRangeSwitch,
    EocButton,
    LevelKnob,
    LevelRangeSwitch,
    ShapeSwitch,
    TriggerButton,
    ParameterCount
  };

  enum InputIds { CurveCvInput, DeferInput, DurationCvInput, LevelCvInput, EnvelopeInput, TriggerInput, InputCount };

  enum OutputIds { ActiveOutput, EocOutput, EnvelopeOutput, OutputCount };

private:
  auto activeButton() const -> bool { return buttonIsPressed(this, ActiveButton); }

  auto deferIsHigh() const -> bool override {
    return inputIsHigh(this, DeferInput) || buttonIsPressed(this, DeferButton);
  }

  auto duration() const -> float override {
    return selectableDuration(this, DurationKnob, DurationCvInput, DurationRangeSwitch);
  }

  auto envelopeIn() const -> float override { return inputVoltage(this, EnvelopeInput); }

  auto eocButton() const -> bool { return buttonIsPressed(this, EocButton); }

  auto level() const -> float override { return selectableLevel(this, LevelKnob, LevelCvInput, LevelRangeSwitch); }

  void sendActive(bool isActive) override {
    auto const voltage = unipolarSignalRange.scale(isActive || activeButton());
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void sendEoc(bool isEoc) override {
    auto const voltage = unipolarSignalRange.scale(isEoc || eocButton());
    outputs[EocOutput].setVoltage(voltage);
  }

  void sendOut(float voltage) override { outputs[EnvelopeOutput].setVoltage(voltage); }

  auto taper(float input) const -> float override {
    auto const curvature = dhe::curvature(this, CurveKnob, CurveCvInput);
    auto const taper = selectedTaper(this, ShapeSwitch);
    return taper->apply(input, curvature);
  }

  auto triggerIsHigh() const -> bool override {
    return inputIsHigh(this, TriggerInput) || buttonIsPressed(this, TriggerButton);
  }
};
} // namespace dhe
