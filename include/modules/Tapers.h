#pragma once

#include "Module.h"
#include "util/range.h"

#include <functional>

namespace dhe {

class Tapers : public Module {
public:
  Tapers();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    LevelKnob1,
    LevelAvKnob1,
    LevelRangeSwitch1,
    CurveKnob1,
    CurveAvKnob1,
    ShapeSwitch1,
    LevelKnob2,
    LevelAvKnob2,
    LevelRangeSwitch2,
    Curve2Knob,
    Curve2Av,
    Shape2Switch,
    ParameterCount
  };
  enum InputIds { Level1Cv, Curve1Cv, Level2Cv, Curve2Cv, InputCount };
  enum OutputIds { Taper1Output, Taper2Output, OutputCount };

private:
  std::function<float()> level1Rotation;
  std::function<Range const *()> level1Range;
  std::function<float(float)> taper1;
  std::function<float()> level2Rotation;
  std::function<Range const *()> level2Range;
  std::function<float(float)> taper2;
};

} // namespace dhe
