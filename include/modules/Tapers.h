#pragma once

#include "modules/components/Range.h"
#include "modules/controls/CommonInputs.h"
#include "modules/controls/CurvatureInputs.h"
#include "modules/controls/LevelInputs.h"

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
    auto const taperCurvature = curvature(this, CurveKnob1, CurveCvInput1, CurveAvKnob1);
    return taper->apply(input, taperCurvature);
  }

  auto levelRotation2() const -> float {
    return rotation(params[LevelKnob2], inputs[LevelCvInput2], params[LevelAvKnob2]);
  };
  auto levelRange2() const -> Range const * { return levelRange(params[LevelRangeSwitch2]); }
  auto taper2(float input) const -> float {
    auto const taper = selectedTaper(params[ShapeSwitch2]);
    auto const taperCurvature = curvature(this, CurveKnob2, CurveCvInput2, CurveAvKnob2);
    return taper->apply(input, taperCurvature);
  }
};

} // namespace dhe
