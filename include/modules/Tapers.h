#pragma once

#include "util/Range.h"

#include <engine/Module.hpp>
#include <functional>

namespace dhe {

class Tapers : public rack::engine::Module {
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
    CurveKnob2,
    CurveAv2,
    ShapeSwitch2,
    ParameterCount
  };
  enum InputIds { Level1Cv, Curve1Cv, Level2Cv, CurveCv2, InputCount };
  enum OutputIds { TaperOutput1, Taper2Output, OutputCount };

private:
  std::function<float()> levelRotation1;
  std::function<Range const *()> levelRange1;
  std::function<float(float)> taper1;
  std::function<float()> levelRotation2;
  std::function<Range const *()> levelRange2;
  std::function<float(float)> taper2;
};

} // namespace dhe
