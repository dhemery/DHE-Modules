#include "modules/BoosterStage.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"

#include <string>

namespace dhe {

void StageMachine::process(float sampleTime) {
  auto const newState = identifyState();
  if (state != newState) {
    transition(state, newState);
  }
  state = newState;

  switch (state) {
  case Generating:
    generate(sampleTime);
    break;
  case TrackingLevel:
    sendOut(level());
    break;
  case Deferring:
  case TrackingInput:
    sendOut(envelopeIn());
  }

  advanceEoc(sampleTime);
  sendActive(state == Deferring || state == Generating);
  sendEoc(isEoc);
}

void StageMachine::resetGenerator() {
  startVoltage = envelopeIn();
  stagePhase = 0.F;
}

void StageMachine::generate(float sampleTime) {
  if (stagePhase < 1.F) {
    stagePhase = std::min(1.F, stagePhase + sampleTime / duration());
    sendOut(scale(taper(stagePhase), startVoltage, level()));
    if (stagePhase == 1.F) {
      finishGenerating();
    }
  }
}

void StageMachine::finishGenerating() {
  startEoc();
  state = TrackingLevel;
}

void StageMachine::advanceEoc(float sampleTime) {
  if (eocPhase < 1.F) {
    eocPhase = std::min(1.F, eocPhase + sampleTime / 1e-3F);
    if (eocPhase == 1.F) {
      finishEoc();
    }
  }
}

StageMachine::State StageMachine::identifyState() {
  if (deferIsHigh()) {
    return Deferring;
  }
  if (triggerRise()) {
    return Generating;
  }
  return state;
}

void StageMachine::transition(StageMachine::State fromState, StageMachine::State toState) {
  switch (fromState) {
  case Deferring:
    stopDeferring();
    break;
  case Generating:
    startEoc();
    break;
  default:;
  }

  if (toState == Generating) {
    resetGenerator();
  }
}

bool StageMachine::triggerRise() {
  auto const isHigh = triggerIsHigh();
  auto const isRise = isHigh && !triggerWasHigh;
  triggerWasHigh = isHigh;
  return isRise;
}

void StageMachine::finishEoc() { isEoc = false; }
void StageMachine::startEoc() {
  isEoc = true;
  eocPhase = 0.F;
}
void StageMachine::stopDeferring() {
  triggerWasHigh = false;
  state = TrackingInput;
}

BoosterStage::BoosterStage() {
  config(ParameterCount, InputCount, OutputCount);

  configDurationKnob(this, DurationKnob, DurationRangeSwitch);
  configDurationRangeSwitch(this, DurationRangeSwitch);

  configLevelKnob(this, LevelKnob, LevelRangeSwitch);
  configLevelRangeSwitch(this, LevelRangeSwitch);

  configCurvatureKnob(this, CurveKnob);
  configCurveShapeSwitch(this, ShapeSwitch);

  configButton(this, DeferButton, "DEFER", {"From input", "High"}, 0);
  configButton(this, TriggerButton, "TRIG", {"From input", "High"}, 0);
  configButton(this, ActiveButton, "ACTIVE", {"Generated", "High"}, 0);
  configButton(this, EocButton, "EOC", {"Generated", "High"}, 0);
}
} // namespace dhe
