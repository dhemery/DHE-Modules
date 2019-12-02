#include "Tapers.h"

#include "config/CommonConfig.h"
#include "config/CurvatureConfig.h"
#include "config/LevelConfig.h"

namespace dhe {

Tapers::Tapers() {
  config(ParameterCount, InputCount, OutputCount);

  configLevelKnob(this, LevelKnob1, LevelRangeSwitch1, "Level 1");
  configLevelRangeSwitch(this, LevelRangeSwitch1, "Level 1 range", 0);
  configAttenuverter(this, LevelAvKnob1, "Level 1 CV gain");

  configCurvatureKnob(this, CurveKnob1, "Curvature 1");
  configAttenuverter(this, CurveAvKnob1, "Curvature 1 CV gain");
  configLevelRangeSwitch(this, ShapeSwitch1, "Shape 1");

  configLevelKnob(this, LevelKnob2, LevelRangeSwitch2, "Level 2");
  configLevelRangeSwitch(this, LevelRangeSwitch2, "Level 2 range", 0);
  configAttenuverter(this, LevelAvKnob2, "Level 2 CV gain");

  configCurvatureKnob(this, CurveKnob2, "Curvature 2");
  configAttenuverter(this, CurveAvKnob2, "Curvature 2 CV gain");
  configLevelRangeSwitch(this, ShapeSwitch2, "Shape 2");
}

void Tapers::process(ProcessArgs const & /*args*/) {
  outputs[TaperOutput1].setVoltage(levelRange1()->scale(taper1(levelRotation1())));
  outputs[TaperOutput2].setVoltage(levelRange2()->scale(taper2(levelRotation2())));
}
} // namespace dhe
