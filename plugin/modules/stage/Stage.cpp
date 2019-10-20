#include "Stage.h"

#include "CurvatureConfig.h"
#include "DurationConfig.h"
#include "LevelConfig.h"

namespace dhe {
Stage::Stage() {
  config(ParameterCount, InputCount, OutputCount);

  configDurationKnob(this, DurationKnob, mediumDurationRange);
  configLevelKnob(this, LevelKnob, unipolarSignalRange);
  configCurvatureKnob(this, CurveKnob);
}

} // namespace dhe
