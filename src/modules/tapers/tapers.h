#pragma once

#include "./tapers-controls.h"
#include "components/range.h"
#include "params/common-config.h"
#include "params/curvature-config.h"
#include "params/level-config.h"
#include "signals/common-inputs.h"
#include "signals/curvature-inputs.h"
#include "signals/level-inputs.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {

class Tapers : public rack::engine::Module {
  using Controls = TapersControls;

public:
  Tapers() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_level_knob(this, Controls::LevelKnob1, Controls::LevelRangeSwitch1,
                      "Taper 1 level");
    config_level_range_switch(this, Controls::LevelRangeSwitch1,
                              "Taper 1 level range", 0);
    config_attenuverter(this, Controls::LevelAvKnob1, "Taper 1 level CV gain");
    configInput(Controls::LevelCvInput1, "Taper 1 level CV");

    config_curvature_knob(this, Controls::CurveKnob1, "Taper 1 curvature");
    config_attenuverter(this, Controls::CurveAvKnob1,
                        "Taper 1 curvature CV gain");
    config_curve_shape_switch(this, Controls::ShapeSwitch1, "Taper 1 shape");
    configInput(Controls::CurveCvInput1, "Taper 1 curvature CV");

    configOutput(Controls::TaperOutput1, "Taper 1");

    config_level_knob(this, Controls::LevelKnob2, Controls::LevelRangeSwitch2,
                      "Taper 2 level");
    config_level_range_switch(this, Controls::LevelRangeSwitch2,
                              "Taper 2 level range", 0);
    config_attenuverter(this, Controls::LevelAvKnob2, "Taper 2 level CV gain");
    configInput(Controls::LevelCvInput2, "Taper 2 level CV");

    config_curvature_knob(this, Controls::CurveKnob2, "Taper 2 curvature");
    config_attenuverter(this, Controls::CurveAvKnob2,
                        "Taper 2 curvature CV gain");
    config_curve_shape_switch(this, Controls::ShapeSwitch2, "Taper 2 shape");
    configInput(Controls::CurveCvInput2, "Taper 2 curvature CV");

    configOutput(Controls::TaperOutput2, "Taper 2");
  }

  void process(ProcessArgs const & /*args*/) override {
    outputs[Controls::TaperOutput1].setVoltage(
        level_range_1().scale(taper_1(level_rotation_1())));
    outputs[Controls::TaperOutput2].setVoltage(
        level_range_2().scale(taper_2(level_rotation_2())));
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto level_rotation_1() const -> float {
    return rotation(params[Controls::LevelKnob1],
                    inputs[Controls::LevelCvInput1],
                    params[Controls::LevelAvKnob1]);
  };
  auto level_range_1() const -> Range {
    return level_range(params[Controls::LevelRangeSwitch1]);
  }

  auto taper_1(float input) const -> float {
    auto const &taper = selected_taper(params[Controls::ShapeSwitch1]);
    auto const taper_curvature =
        curvature(params[Controls::CurveKnob1], inputs[Controls::CurveCvInput1],
                  params[Controls::CurveAvKnob1]);
    return taper.apply(input, taper_curvature);
  }

  auto level_rotation_2() const -> float {
    return rotation(params[Controls::LevelKnob2],
                    inputs[Controls::LevelCvInput2],
                    params[Controls::LevelAvKnob2]);
  };
  auto level_range_2() const -> Range {
    return level_range(params[Controls::LevelRangeSwitch2]);
  }

  auto taper_2(float input) const -> float {
    const auto &taper = selected_taper(params[Controls::ShapeSwitch2]);
    auto const taper_curvature =
        curvature(params[Controls::CurveKnob2], inputs[Controls::CurveCvInput2],
                  params[Controls::CurveAvKnob2]);
    return taper.apply(input, taper_curvature);
  }
};

} // namespace tapers
} // namespace dhe
