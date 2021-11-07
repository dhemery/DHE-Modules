#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/curvature-config.h"
#include "params/presets.h"
#include "signals/common-inputs.h"
#include "signals/curvature-inputs.h"
#include "signals/levels.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {

class Module : public rack::engine::Module {
public:
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    auto *level_knob_1 =
        Knob::config<Bipolar>(this, Param::Level1, "Taper 1 level", 0.F);
    Picker::config<Levels>(this, Param::LevelRange1, "Taper 1 level range",
                           Levels::Bipolar)
        ->on_change(
            [level_knob_1](Range r) { level_knob_1->set_display_range(r); });
    Knob::config<Attenuverter>(this, Param::LevelAv1, "Taper 1 level CV gain",
                               0.F);
    configInput(Input::LevelCv1, "Taper 1 level CV");

    config_curvature_knob(this, Param::Curvature1, "Taper 1 curvature");
    Knob::config<Attenuverter>(this, Param::CurvatureAv1,
                               "Taper 1 curvature CV gain", 0.F);
    config_curve_shape_switch(this, Param::Shape1, "Taper 1 shape");
    configInput(Input::CurvatureCv1, "Taper 1 curvature CV");

    configOutput(Output::Taper1, "Taper 1");

    auto *level_knob_2 =
        Knob::config<Bipolar>(this, Param::Level2, "Taper 2 level", 0.F);
    Picker::config<Levels>(this, Param::LevelRange2, "Taper 2 level range",
                           Levels::Bipolar)
        ->on_change(
            [level_knob_2](Range r) { level_knob_2->set_display_range(r); });
    Knob::config<Attenuverter>(this, Param::LevelAv2, "Taper 2 level CV gain",
                               0.F);
    configInput(Input::LevelCv2, "Taper 2 level CV");

    config_curvature_knob(this, Param::Curvature2, "Taper 2 curvature");
    Knob::config<Attenuverter>(this, Param::CurvatureAv2,
                               "Taper 2 curvature CV gain", 0.F);
    config_curve_shape_switch(this, Param::Shape2, "Taper 2 shape");
    configInput(Input::CurvatureCv2, "Taper 2 curvature CV");

    configOutput(Output::Taper2, "Taper 2");
  }

  void process(ProcessArgs const & /*args*/) override {
    outputs[Output::Taper1].setVoltage(
        level_range_1().scale(taper_1(level_rotation_1())));
    outputs[Output::Taper2].setVoltage(
        level_range_2().scale(taper_2(level_rotation_2())));
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto level_range(int id) const -> Range {
    auto const selection = position_of(params[id]);
    return Levels::items()[selection];
  }

  auto level_rotation_1() const -> float {
    return rotation(params[Param::Level1], inputs[Input::LevelCv1],
                    params[Param::LevelAv1]);
  };
  auto level_range_1() const -> Range {
    return level_range(Param::LevelRange1);
  }

  auto taper_1(float input) const -> float {
    auto const &taper = selected_taper(params[Param::Shape1]);
    auto const taper_curvature =
        curvature(params[Param::Curvature1], inputs[Input::CurvatureCv1],
                  params[Param::CurvatureAv1]);
    return taper.apply(input, taper_curvature);
  }

  auto level_rotation_2() const -> float {
    return rotation(params[Param::Level2], inputs[Input::LevelCv2],
                    params[Param::LevelAv2]);
  };
  auto level_range_2() const -> Range {
    return level_range(Param::LevelRange2);
  }

  auto taper_2(float input) const -> float {
    const auto &taper = selected_taper(params[Param::Shape2]);
    auto const taper_curvature =
        curvature(params[Param::Curvature2], inputs[Input::CurvatureCv2],
                  params[Param::CurvatureAv2]);
    return taper.apply(input, taper_curvature);
  }
};

} // namespace tapers
} // namespace dhe
