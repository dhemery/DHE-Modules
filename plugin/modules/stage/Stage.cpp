#include "Stage.h"

#include "config/CurvatureConfig.h"
#include "config/DurationConfig.h"
#include "config/LevelConfig.h"

namespace dhe {
Stage::Stage() {
  config(ParameterCount, InputCount, OutputCount);

  configDurationKnob(this, DurationKnob, mediumDurationRange);
  configLevelKnob(this, LevelKnob, unipolarSignalRange);
  configCurvatureKnob(this, CurveKnob);
}

} // namespace dhe
