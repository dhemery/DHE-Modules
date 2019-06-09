#pragma once
#include "envelopes/StageStateMachine.h"
#include "modules/components/Taper.h"
#include "modules/controls/CommonInputs.h"
#include "modules/controls/CurvatureInputs.h"
#include "modules/controls/DurationInputs.h"
#include "modules/controls/LevelInputs.h"

#include <engine/Module.hpp>

namespace dhe {

class BoosterStage : public rack::engine::Module {
public:
  BoosterStage();
  void process(const ProcessArgs &args) override;

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

  auto deferIsHigh() const -> bool { return inputIsHigh(this, DeferInput) || buttonIsPressed(this, DeferButton); }

  auto duration() const -> float {
    return selectableDuration(this, DurationKnob, DurationCvInput, DurationRangeSwitch);
  }

  auto envelopeIn() const -> float { return inputVoltage(this, EnvelopeInput); }

  auto eocButton() const -> bool { return buttonIsPressed(this, EocButton); }

  void finishEoc() { isEoc = false; }

  void forward() { sendOut(envelopeIn()); }

  void generate() { sendOut(scale(taper(stagePhase), startVoltage, level())); }

  auto level() const -> float { return selectableLevel(this, LevelKnob, LevelCvInput, LevelRangeSwitch); }

  void resetTrigger() { triggerWasHigh = false; }

  void sendActive() {
    auto const voltage = unipolarSignalRange.scale(isActive || activeButton());
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void sendEoc() {
    auto const voltage = unipolarSignalRange.scale(isEoc || eocButton());
    outputs[EocOutput].setVoltage(voltage);
  }

  void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  void startEoc() {
    isEoc = true;
    eocPhase = 0.F;
  }

  void startTrackingInput() {
    isTrackingInput = true;
    isActive = false;
  }

  auto taper(float input) const -> float {
    auto const curvature = dhe::curvature(this, CurveKnob, CurveCvInput);
    auto const taper = selectedTaper(this, ShapeSwitch);
    return taper->apply(input, curvature);
  }

  void trackInput() { sendOut(envelopeIn()); }

  void trackLevel() { sendOut(level()); }

  auto triggerIsHigh() const -> bool { return inputIsHigh(this, TriggerInput) || buttonIsPressed(this, TriggerButton); }

  bool checkDeferGate();
  void startDeferring();
  void finishDeferring();

  void checkStageTrigger();
  void startGenerating();
  void advancePhase(float sampleTime);
  void finishGenerating();

  void advanceEoc(float sampleTime);

  bool deferWasHigh = false;
  float eocPhase = 1.F;
  bool isActive = false;
  bool isEoc = false;
  bool isGenerating = false;
  bool isTrackingInput = false;
  float stagePhase = 0.F;
  float startVoltage = 0.F;
  bool triggerWasHigh = false;
  void startTrackingLevel();
};
} // namespace dhe
