#pragma once

#include <functional>

#include "Module.h"
#include "util/range.h"

namespace dhe {

class Tapers : public Module {
public:
  Tapers();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    Level1Knob,
    Level1Av,
    Range1Switch,
    Curve1Knob,
    Curve1Av,
    Shape1Switch,
    Level2Knob,
    Level2Av,
    Range2Switch,
    Curve2Knob,
    Curve2Av,
    Shape2Switch,
    ParameterCount
  };
  enum InputIds { Level1Cv, Curve1Cv, Level2Cv, Curve2Cv, InputCount };
  enum OutputIds { Out1, Out2, OutputCount };

private:
  std::function<float()> level1Rotation;
  std::function<Range const *()> level1Range;
  std::function<float(float)> taper1;
  std::function<float()> level2Rotation;
  std::function<Range const *()> level2Range;
  std::function<float(float)> taper2;
};

} // namespace dhe
