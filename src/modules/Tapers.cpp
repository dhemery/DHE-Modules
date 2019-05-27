#include "modules/Tapers.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Curvature.h"
#include "modules/controls/Level.h"

namespace dhe {

Tapers::Tapers() {
  config(ParameterCount, InputCount, OutputCount);

  level::configKnob(this, LevelKnob1, LevelRangeSwitch1, "Level 1");
  level::configSwitch(this, LevelRangeSwitch1, "Level 1 range", 0);
  attenuverter::config(this, LevelAvKnob1, "Level 1 CV gain");

  curvature::configKnob(this, CurveKnob1, "Curvature 1");
  attenuverter::config(this, CurveAvKnob1, "Curvature 1 CV gain");
  curvature::configSwitch(this, ShapeSwitch1, "Shape 1");

  level::configKnob(this, LevelKnob2, LevelRangeSwitch2, "Level 2");
  level::configSwitch(this, LevelRangeSwitch2, "Level 2 range", 0);
  attenuverter::config(this, LevelAvKnob2, "Level 2 CV gain");

  curvature::configKnob(this, CurveKnob2, "Curvature 2");
  attenuverter::config(this, CurveAv2, "Curvature 2 CV gain");
  curvature::configSwitch(this, ShapeSwitch2, "Shape 2");

  levelRotation1 = knob::rotation(this, LevelKnob1, Level1Cv, LevelAvKnob1);
  levelRange1 = range::selected<2>(this, LevelRangeSwitch1, level::ranges);
  taper1 = taper::withSelectableShape(this, CurveKnob1, Curve1Cv, CurveAvKnob1, ShapeSwitch1);

  levelRotation2 = knob::rotation(this, LevelKnob2, Level2Cv, LevelAvKnob2);
  levelRange2 = range::selected<2>(this, LevelRangeSwitch2, level::ranges);
  taper2 = taper::withSelectableShape(this, CurveKnob2, CurveCv2, CurveAv2, ShapeSwitch2);
}

void Tapers::process(const ProcessArgs & /*args*/) {
  outputs[TaperOutput1].setVoltage(levelRange1()->scale(taper1(levelRotation1())));
  outputs[Taper2Output].setVoltage(levelRange2()->scale(taper2(levelRotation2())));
}
} // namespace dhe
