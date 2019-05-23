#include "modules/Tapers.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Curvature.h"
#include "modules/controls/Level.h"

#include "util/sigmoid.h"

namespace dhe {

auto taper(float level, Range const *range,
           std::function<float(float)> const &taper) -> float {
  return range->scale(taper(level));
}

Tapers::Tapers() {
  config(ParameterCount, InputCount, OutputCount);

  level::configKnob(this, Level1Knob, Range1Switch, "Level 1");
  level::configSwitch(this, Range1Switch, "Level 1 range", 0);
  configCvGain(Level1Av, "Level 1");

  curvature::configKnob(this, Curve1Knob, "Curvature 1");
  configCvGain(Curve1Av, "Curvature 1");
  curvature::configSwitch(this, Shape1Switch, "Shape 1");

  level::configKnob(this, Level2Knob, Range2Switch, "Level 2");
  level::configSwitch(this, Range2Switch, "Level 2 range", 0);
  configCvGain(Level2Av, "Level 2");

  curvature::configKnob(this, Curve2Knob, "Curvature 2");
  configCvGain(Curve2Av, "Curvature 2");
  curvature::configSwitch(this, Shape2Switch, "Shape 2");

  using namespace control;

  level1Rotation = knob::rotation(this, Level1Knob, Level1Cv, Level1Av);
  level1Range = range::selection<2>(this, Range1Switch, level::ranges);
  taper1 = curvature::withSelectableShape(this, Curve1Knob, Curve1Cv, Curve1Av,
                                          Shape1Switch);

  level2Rotation = knob::rotation(this, Level2Knob, Level2Cv, Level2Av);
  level2Range = range::selection<2>(this, Range2Switch, level::ranges);
  taper2 = curvature::withSelectableShape(this, Curve2Knob, Curve2Cv, Curve2Av,
                                          Shape2Switch);
}

void Tapers::process(const ProcessArgs &args) {
  outputs[Out1].setVoltage(level1Range()->scale(taper1(level1Rotation())));
  outputs[Out2].setVoltage(level2Range()->scale(taper2(level2Rotation())));
}
} // namespace dhe
