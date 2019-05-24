#include "modules/Tapers.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Curvature.h"
#include "modules/controls/Level.h"
#include "util/sigmoid.h"

namespace dhe {

auto taper(float level, Range const *range, std::function<float(float)> const &taper) -> float {
  return range->scale(taper(level));
}

Tapers::Tapers() {
  config(ParameterCount, InputCount, OutputCount);

  level::configKnob(this, LevelKnob1, LevelRangeSwitch1, "Level 1");
  level::configSwitch(this, LevelRangeSwitch1, "Level 1 range", 0);
  configCvGain(LevelAvKnob1, "Level 1");

  curvature::configKnob(this, CurveKnob1, "Curvature 1");
  configCvGain(CurveAvKnob1, "Curvature 1");
  curvature::configSwitch(this, ShapeSwitch1, "Shape 1");

  level::configKnob(this, LevelKnob2, LevelRangeSwitch2, "Level 2");
  level::configSwitch(this, LevelRangeSwitch2, "Level 2 range", 0);
  configCvGain(LevelAvKnob2, "Level 2");

  curvature::configKnob(this, Curve2Knob, "Curvature 2");
  configCvGain(Curve2Av, "Curvature 2");
  curvature::configSwitch(this, Shape2Switch, "Shape 2");

  using namespace control;

  level1Rotation = knob::rotation(this, LevelKnob1, Level1Cv, LevelAvKnob1);
  level1Range = range::selection<2>(this, LevelRangeSwitch1, level::ranges);
  taper1 = curvature::withSelectableShape(this, CurveKnob1, Curve1Cv, CurveAvKnob1, ShapeSwitch1);

  level2Rotation = knob::rotation(this, LevelKnob2, Level2Cv, LevelAvKnob2);
  level2Range = range::selection<2>(this, LevelRangeSwitch2, level::ranges);
  taper2 = curvature::withSelectableShape(this, Curve2Knob, Curve2Cv, Curve2Av, Shape2Switch);
}

void Tapers::process(const ProcessArgs &args) {
  outputs[Taper1Output].setVoltage(level1Range()->scale(taper1(level1Rotation())));
  outputs[Taper2Output].setVoltage(level2Range()->scale(taper2(level2Rotation())));
}
} // namespace dhe
