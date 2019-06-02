#include "modules/Stage.h"

#include "modules/controls/CurvatureControls.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

namespace dhe {
Stage::Stage() {
  config(ParameterCount, InputCount, OutputCount);

  duration::configKnob(this, DurationKnob, duration::mediumRange);
  level::configKnob(this, LevelKnob, level::unipolarRange);
  configCurvatureKnob(this, CurveKnob);

  stateMachine.start();
}

void Stage::process(const ProcessArgs &args) { stateMachine.step(args.sampleTime); }

} // namespace dhe
