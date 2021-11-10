#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/shapes.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {

class Module : public rack::engine::Module {
public:
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    auto *level_knob_1 =
        Knob::config<Voltages>(this, Param::Level1, "Taper 1 level");
    auto select_level_1_range = [level_knob_1](int range_index) {
      level_knob_1->mapper().select_range(range_index);
    };
    Switch::config<Voltages>(this, Param::LevelRange1, "Taper 1 level range",
                             Voltages::Bipolar)
        ->on_change(select_level_1_range);
    Knob::config<Attenuverter>(this, Param::LevelAv1, "Taper 1 level CV gain");
    configInput(Input::LevelCv1, "Taper 1 level CV");

    Knob::config<Curvature>(this, Param::Curvature1, "Taper 1 curvature");
    Knob::config<Attenuverter>(this, Param::CurvatureAv1,
                               "Taper 1 curvature CV gain");
    Switch::config<Shapes>(this, Param::Shape1, "Taper 1 shape", Shapes::J);
    configInput(Input::CurvatureCv1, "Taper 1 curvature CV");

    configOutput(Output::Taper1, "Taper 1");

    auto *level_knob_2 =
        Knob::config<Voltages>(this, Param::Level2, "Taper 2 level");
    auto select_level_2_range = [level_knob_2](int range_index) {
      level_knob_2->mapper().select_range(range_index);
    };
    Switch::config<Voltages>(this, Param::LevelRange2, "Taper 2 level range",
                             Voltages::Bipolar)
        ->on_change(select_level_2_range);
    Knob::config<Attenuverter>(this, Param::LevelAv2, "Taper 2 level CV gain");
    configInput(Input::LevelCv2, "Taper 2 level CV");

    Knob::config<Curvature>(this, Param::Curvature2, "Taper 2 curvature");
    Knob::config<Attenuverter>(this, Param::CurvatureAv2,
                               "Taper 2 curvature CV gain");
    Switch::config<Shapes>(this, Param::Shape2, "Taper 2 shape", Shapes::J);
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
  inline auto taper(float input, int knob_id, int cv_id, int av_id,
                    int switch_id) const -> float {
    const auto &taper = Shapes::select(position_of(params[switch_id]));
    auto const curvature = Curvature::value(
        rotation(params[knob_id], inputs[cv_id], params[av_id]));
    return taper.apply(input, curvature);
  }

  auto level_range(int id) const -> Range {
    auto const selection = position_of(params[id]);
    return Voltages::select(selection);
  }

  auto level_rotation_1() const -> float {
    return rotation(params[Param::Level1], inputs[Input::LevelCv1],
                    params[Param::LevelAv1]);
  };

  auto level_range_1() const -> Range {
    return level_range(Param::LevelRange1);
  }

  auto taper_1(float input) const -> float {
    return taper(input, Param::Curvature1, Input::CurvatureCv1,
                 Param::CurvatureAv1, Param::Shape1);
  }

  auto level_rotation_2() const -> float {
    return rotation(params[Param::Level2], inputs[Input::LevelCv2],
                    params[Param::LevelAv2]);
  };
  auto level_range_2() const -> Range {
    return level_range(Param::LevelRange2);
  }

  auto taper_2(float input) const -> float {
    return taper(input, Param::Curvature2, Input::CurvatureCv2,
                 Param::CurvatureAv2, Param::Shape2);
  }
};

} // namespace tapers
} // namespace dhe
