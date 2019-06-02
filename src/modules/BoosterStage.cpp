#include "modules/BoosterStage.h"

#include "modules/controls/Controls.h"
#include "modules/controls/CurvatureControls.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

#include <array>
#include <string>

namespace dhe {

BoosterStage::BoosterStage() {
  config(ParameterCount, InputCount, OutputCount);

  duration::configKnob(this, DurationKnob, DurationRangeSwitch);
  duration::configSwitch(this, DurationRangeSwitch);

  level::configKnob(this, LevelKnob, LevelRangeSwitch);
  level::configSwitch(this, LevelRangeSwitch);

  configCurvatureKnob(this, CurveKnob);
  configShapeSwitch(this, ShapeSwitch);

  configButton(this, DeferButton, "DEFER", {"From input", "High"}, 0);
  configButton(this, TriggerButton, "TRIG", {"From input", "High"}, 0);
  configButton(this, ActiveButton, "ACTIVE", {"Generated", "High"}, 0);
  configButton(this, EocButton, "EOC", {"Generated", "High"}, 0);

  stateMachine.start();
}

void BoosterStage::process(const ProcessArgs &args) {
  stateMachine.step(args.sampleTime);

  sendActive();
  sendEoc();
}

} // namespace dhe
