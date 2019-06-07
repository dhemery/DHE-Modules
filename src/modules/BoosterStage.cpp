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
  processEoc(args.sampleTime);

  if (!processDefer()) {
    processTrigger();
    processGenerator(args.sampleTime);
  }

  sendActive();
  sendEoc();
}

bool BoosterStage::processDefer() {
  auto const deferIsHigh = BoosterStage::deferIsHigh();
  if (deferIsHigh) {
    trackInput();
    if (!deferWasHigh) { // On defer rise…
      beginDeferring();
    }
  } else if (deferWasHigh) { // On defer fall…
    stopDeferring();
  }
  deferWasHigh = deferIsHigh;

  return deferIsHigh;
}

void BoosterStage::beginDeferring() {
  setActive(true);
}

void BoosterStage::trackInput() { sendOut(envelopeIn()); }

void BoosterStage::stopDeferring() {
  setActive(false);
  triggerWasHigh = false;
  startVoltage = envelopeIn();
}

void BoosterStage::processTrigger() {
  auto const triggerIsHigh = BoosterStage::triggerIsHigh();
  if (triggerIsHigh && !triggerWasHigh) { // On trigger rise…
    startGenerating();
  }
  triggerWasHigh = triggerIsHigh;
}

void BoosterStage::processGenerator(float sampleTime) {
  if (stagePhase < 1.F) {
    stagePhase = std::min(1.F, stagePhase + sampleTime / duration());
    generate(stagePhase);
    if (stagePhase == 1.F) {
      finishGenerating();
    }
  }
}

void BoosterStage::startGenerating() {
  setActive(true);
  stagePhase = 0.F;
}
void BoosterStage::finishGenerating() {
  setActive(false);
  setEoc(true);
  eocPhase = 0.F;
}

void BoosterStage::processEoc(float sampleTime) {
  if (eocPhase < 1.F) {
    eocPhase = std::min(1.F, eocPhase + sampleTime / 1e-3F);
    generate(eocPhase);
    if (eocPhase == 1.F) {
      setEoc(false);
    }
  }
}
} // namespace dhe
