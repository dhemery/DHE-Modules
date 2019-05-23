#include "modules/Tapers.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"
#include "modules/params/LevelParams.h"

#include "util/sigmoid.h"
#include "util/signal.h"

namespace dhe {

auto taper(float level, Range const *range, CurvatureControl *curvature)
    -> float {
  auto tapered = curvature->taper(level);
  return range->scale(tapered);
}

Tapers::Tapers() {
  config(ParameterCount, InputCount, OutputCount);

  level::configKnob(this, Level1Knob, Range1Switch, "Level 1");
  level::configSwitch(this, Range1Switch, "Level 1 range");

  configCvGain(Level1Av, "Level 1");
  configParam(Curve1Knob, 0.f, 1.f, 0.5f, "Curvature 1", "%", 0.f, 100.f, 0.f);
  configCvGain(Curve1Av, "Curvature 1");
  configParam(Shape1Switch, 0.f, 1.f, 0.f, "Shape 1");

  level::configKnob(this, Level2Knob, Range2Switch, "Level 2");
  level::configSwitch(this, Range2Switch, "Level 2 range");
  configCvGain(Level2Av, "Level 2");
  configParam(Curve2Knob, 0.f, 1.f, 0.5f, "Curvature 2", "%", 0.f, 100.f, 0.f);
  configCvGain(Curve2Av, "Curvature 2");
  configParam(Shape2Switch, 0.f, 1.f, 0.f, "Shape 2");

  using namespace control;

  level1Rotation = knob::rotation(this, Level1Knob, Level1Cv, Level1Av);
  level1Range = range::selection<2>(this, Range1Switch, level::ranges);
  curvature1 = std::unique_ptr<CurvatureControl>(
      new CurvatureControl(params[Curve1Knob], params[Shape1Switch],
                           inputs[Curve1Cv], params[Curve1Av]));

  level2Rotation = knob::rotation(this, Level2Knob, Level2Cv, Level2Av);
  level2Range = range::selection<2>(this, Range2Switch, level::ranges);
  curvature2 = std::unique_ptr<CurvatureControl>(
      new CurvatureControl(params[Curve2Knob], params[Shape2Switch],
                           inputs[Curve2Cv], params[Curve2Av]));
}

void Tapers::process(const ProcessArgs &args) {
  outputs[Out1].setVoltage(
      taper(level1Rotation(), level1Range(), curvature1.get()));
  outputs[Out2].setVoltage(
      taper(level2Rotation(), level2Range(), curvature2.get()));
}
} // namespace dhe
