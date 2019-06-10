#include "modules/Stage.h"

#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"

namespace dhe {
Stage::Stage() {
  config(ParameterCount, InputCount, OutputCount);

  configDurationKnob(this, DurationKnob, mediumDurationRange);
  configLevelKnob(this, LevelKnob, unipolarSignalRange);
  configCurvatureKnob(this, CurveKnob);
}

} // namespace dhe
