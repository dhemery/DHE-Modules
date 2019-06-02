#pragma once

#include "modules/controls/CurvatureControls.h"
#include "modules/controls/Inputs.h"
#include "modules/controls/Level.h"
#include "modules/controls/TaperControls.h"
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
    CurveAvKnob2,
    ShapeSwitch2,
    ParameterCount
  };
  enum InputIds { LevelCvInput1, CurveCvInput1, LevelCvInput2, CurveCvInput2, InputCount };
  enum OutputIds { TaperOutput1, TaperOutput2, OutputCount };

private:
  auto levelRotation1() const -> float { return rotation(this, LevelKnob1, LevelCvInput1, LevelAvKnob1); };
  auto levelRange1() const -> Range const * { return selectedRange<2>(this, LevelRangeSwitch1, level::ranges); }
  auto taper1(float input) const -> float {
    auto const taper = selectedTaper(this, ShapeSwitch1);
    auto const taperCurvature = curvature(this, CurveKnob1, CurveCvInput1, CurveAvKnob1);
    return taper->apply(input, taperCurvature);
  }

  auto levelRotation2() const -> float { return rotation(this, LevelKnob2, LevelCvInput2, LevelAvKnob2); };
  auto levelRange2() const -> Range const * { return selectedRange<2>(this, LevelRangeSwitch2, level::ranges); }
  auto taper2(float input) const -> float {
    auto const taper = selectedTaper(this, ShapeSwitch2);
    auto const taperCurvature = curvature(this, CurveKnob2, CurveCvInput2, CurveAvKnob2);
    return taper->apply(input, taperCurvature);
  }
};

} // namespace dhe
