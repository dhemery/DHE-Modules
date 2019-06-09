#include "modules/BoosterStage.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"

#include <array>
#include <ldap.h>
#include <string>

namespace dhe {

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

void BoosterStage::process(const ProcessArgs &args) {
  if (!checkDeferGate()) {
    checkStageTrigger();
    if (isGenerating) {
      advancePhase(args.sampleTime);
    } else if (isTrackingInput) {
      trackInput();
    } else {
      trackLevel();
    }
  }

  advanceEoc(args.sampleTime);
  sendActive();
  sendEoc();
}

bool BoosterStage::checkDeferGate() {
  auto const deferIsHigh = BoosterStage::deferIsHigh();
  if (deferIsHigh) {
    trackInput();
    if (!deferWasHigh) { // On defer rise…
      startDeferring();
    }
  } else if (deferWasHigh) { // On defer fall…
    finishDeferring();
  }
  deferWasHigh = deferIsHigh;

  return deferIsHigh;
}

void BoosterStage::startDeferring() {
  isGenerating = false;
  isActive = true;
}

void BoosterStage::finishDeferring() {
  isActive = false;
  resetTrigger();
  startTrackingInput();
}

void BoosterStage::checkStageTrigger() {
  auto const triggerIsHigh = BoosterStage::triggerIsHigh();
  if (triggerIsHigh && !triggerWasHigh) { // On trigger rise…
    startGenerating();
  }
  triggerWasHigh = triggerIsHigh;
}

void BoosterStage::startGenerating() {
  isActive = true;
  isGenerating = true;
  startVoltage = envelopeIn();
  stagePhase = 0.F;
}

void BoosterStage::advancePhase(float sampleTime) {
  if (stagePhase < 1.F) {
    stagePhase = std::min(1.F, stagePhase + sampleTime / duration());
    if (stagePhase == 1.F) {
      finishGenerating();
    }
  }
  generate();
}

void BoosterStage::finishGenerating() {
  isGenerating = false;
  startEoc();
  startTrackingLevel();
}

void BoosterStage::startTrackingLevel() {
  isActive = false;
  isTrackingInput = false;
}

void BoosterStage::advanceEoc(float sampleTime) {
  if (eocPhase < 1.F) {
    eocPhase = std::min(1.F, eocPhase + sampleTime / 1e-3F);
    if (eocPhase == 1.F) {
      finishEoc();
    }
  }
}
} // namespace dhe
