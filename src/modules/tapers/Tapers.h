#pragma once

#include "TapersControls.h"
#include "components/Range.h"
#include "config/CommonConfig.h"
#include "config/CurvatureConfig.h"
#include "config/LevelConfig.h"
#include "controls/CommonInputs.h"
#include "controls/CurvatureInputs.h"
#include "controls/LevelInputs.h"

#include <engine/Module.hpp>

namespace dhe {
namespace tapers {

class Tapers : public rack::engine::Module {
  using Controls = TapersControls;

public:
  Tapers() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_level_knob(this, Controls::LevelKnob1, Controls::LevelRangeSwitch1,
                      "Level 1");
    config_level_range_switch(this, Controls::LevelRangeSwitch1,
                              "Level 1 range", 0);
    config_attenuverter(this, Controls::LevelAvKnob1, "Level 1 CV gain");

    config_curvature_knob(this, Controls::CurveKnob1, "Curvature 1");
    config_attenuverter(this, Controls::CurveAvKnob1, "Curvature 1 CV gain");
    config_level_range_switch(this, Controls::ShapeSwitch1, "Shape 1");

    config_level_knob(this, Controls::LevelKnob2, Controls::LevelRangeSwitch2,
                      "Level 2");
    config_level_range_switch(this, Controls::LevelRangeSwitch2,
                              "Level 2 range", 0);
    config_attenuverter(this, Controls::LevelAvKnob2, "Level 2 CV gain");

    config_curvature_knob(this, Controls::CurveKnob2, "Curvature 2");
    config_attenuverter(this, Controls::CurveAvKnob2, "Curvature 2 CV gain");
    config_level_range_switch(this, Controls::ShapeSwitch2, "Shape 2");
  }

  void process(ProcessArgs const & /*args*/) override {
    outputs[Controls::TaperOutput1].setVoltage(
        levelRange1()->scale(taper1(levelRotation1())));
    outputs[Controls::TaperOutput2].setVoltage(
        levelRange2()->scale(taper2(levelRotation2())));
  }

private:
  auto levelRotation1() const -> float {
    return rotation(params[Controls::LevelKnob1],
                    inputs[Controls::LevelCvInput1],
                    params[Controls::LevelAvKnob1]);
  };
  auto levelRange1() const -> Range const * {
    return levelRange(params[Controls::LevelRangeSwitch1]);
  }
  auto taper1(float input) const -> float {
    auto const *const taper = selected_taper(params[Controls::ShapeSwitch1]);
    auto const taperCurvature =
        curvature(params[Controls::CurveKnob1], inputs[Controls::CurveCvInput1],
                  params[Controls::CurveAvKnob1]);
    return taper->apply(input, taperCurvature);
  }

  auto levelRotation2() const -> float {
    return rotation(params[Controls::LevelKnob2],
                    inputs[Controls::LevelCvInput2],
                    params[Controls::LevelAvKnob2]);
  };
  auto levelRange2() const -> Range const * {
    return levelRange(params[Controls::LevelRangeSwitch2]);
  }
  auto taper2(float input) const -> float {
    const auto *const taper = selected_taper(params[Controls::ShapeSwitch2]);
    auto const taperCurvature =
        curvature(params[Controls::CurveKnob2], inputs[Controls::CurveCvInput2],
                  params[Controls::CurveAvKnob2]);
    return taper->apply(input, taperCurvature);
  }
};

} // namespace tapers
} // namespace dhe
