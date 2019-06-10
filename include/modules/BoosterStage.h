#pragma once
#include "envelopes/StageStateMachine.h"
#include "modules/components/Taper.h"
#include "modules/controls/CommonInputs.h"
#include "modules/controls/CurvatureInputs.h"
#include "modules/controls/DurationInputs.h"
#include "modules/controls/LevelInputs.h"

#include <engine/Module.hpp>

namespace dhe {

class StageMachine {
protected:
  void process(float sampleTime);

  virtual auto envelopeIn() const -> float = 0;
  virtual auto level() const -> float = 0;
  virtual auto deferIsHigh() const -> bool = 0;
  virtual auto duration() const -> float = 0;
  virtual void sendActive(bool isActive) = 0;
  virtual void sendEoc(bool isActive) = 0;
  virtual void sendOut(float voltage) = 0;
  virtual auto taper(float input) const -> float = 0;
  virtual auto triggerIsHigh() const -> bool = 0;

private:
  enum State {
    Deferring,
    Generating,
    TrackingInput,
    TrackingLevel,
  };

  void advanceEoc(float sampleTime);
  void finishEoc();
  void finishGenerating();
  void generate(float sampleTime);
  auto identifyState() -> State;
  void resetGenerator();
  void startEoc();
  void stopDeferring();
  void transition(State fromState, State const toState);
  auto triggerRise() -> bool;

  float eocPhase = 1.F;
  bool isEoc = false;
  float stagePhase = 0.F;
  float startVoltage = 0.F;
  State state = TrackingInput;
  bool triggerWasHigh = false;
};

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
