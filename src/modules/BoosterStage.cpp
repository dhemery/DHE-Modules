#include "modules/BoosterStage.h"

#include "modules/controls/Config.h"
#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"

#include <array>
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

  stateMachine.start();
}

void BoosterStage::process(const ProcessArgs &args) {
  stateMachine.step(args.sampleTime);

  sendActive();
  sendEoc();
}

} // namespace dhe
