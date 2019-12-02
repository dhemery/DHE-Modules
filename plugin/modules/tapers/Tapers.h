#pragma once

#include "components/Range.h"
#include "controls/CommonInputs.h"
#include "controls/CurvatureInputs.h"
#include "controls/LevelInputs.h"

#include <engine/Module.hpp>

namespace dhe {

class Tapers : public rack::engine::Module {
public:
  Tapers();

  void process(ProcessArgs const &args) override;

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
    CurveAvKnob2,
    ShapeSwitch2,
    ParameterCount
  };
  enum InputIds { LevelCvInput1, CurveCvInput1, LevelCvInput2, CurveCvInput2, InputCount };
  enum OutputIds { TaperOutput1, TaperOutput2, OutputCount };

private:
  auto levelRotation1() const -> float {
    return rotation(params[LevelKnob1], inputs[LevelCvInput1], params[LevelAvKnob1]);
  };
  auto levelRange1() const -> Range const * { return levelRange(params[LevelRangeSwitch1]); }
  auto taper1(float input) const -> float {
    auto const taper = selectedTaper(params[ShapeSwitch1]);
    auto const taperCurvature = curvature(params[CurveKnob1], inputs[CurveCvInput1], params[CurveAvKnob1]);
    return taper->apply(input, taperCurvature);
  }

  auto levelRotation2() const -> float {
    return rotation(params[LevelKnob2], inputs[LevelCvInput2], params[LevelAvKnob2]);
  };
  auto levelRange2() const -> Range const * { return levelRange(params[LevelRangeSwitch2]); }
  auto taper2(float input) const -> float {
    auto const taper = selectedTaper(params[ShapeSwitch2]);
    auto const taperCurvature = curvature(params[CurveKnob2], inputs[CurveCvInput2], params[CurveAvKnob2]);
    return taper->apply(input, taperCurvature);
  }
};

} // namespace dhe
