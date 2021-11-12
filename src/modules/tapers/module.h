#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/curvature.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {

class Module : public rack::engine::Module {
public:
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    auto *level_knob_1 =
        Knob::config<VoltageRanges>(this, Param::Level1, "Taper 1 level");
    auto select_level_1_range = [level_knob_1](VoltageRange range) {
      level_knob_1->mapper().select_range(range);
    };
    Switch::config<VoltageRanges>(this, Param::LevelRange1,
                                  "Taper 1 level select", VoltageRange::Bipolar)
        ->on_change(select_level_1_range);
    Knob::config<Attenuverter>(this, Param::LevelAv1, "Taper 1 level CV gain");
    configInput(Input::LevelCv1, "Taper 1 level CV");

    Knob::config<Curvature>(this, Param::Curvature1, "Taper 1 curvature");
    Knob::config<Attenuverter>(this, Param::CurvatureAv1,
                               "Taper 1 curvature CV gain");
    Switch::config<Shapes>(this, Param::Shape1, "Taper 1 shape", Shape::J);
    configInput(Input::CurvatureCv1, "Taper 1 curvature CV");

    configOutput(Output::Taper1, "Taper 1");

    auto *level_knob_2 =
        Knob::config<VoltageRanges>(this, Param::Level2, "Taper 2 level");
    auto select_level_2_range = [level_knob_2](VoltageRange range) {
      level_knob_2->mapper().select_range(range);
    };
    Switch::config<VoltageRanges>(this, Param::LevelRange2,
                                  "Taper 2 level select", VoltageRange::Bipolar)
        ->on_change(select_level_2_range);
    Knob::config<Attenuverter>(this, Param::LevelAv2, "Taper 2 level CV gain");
    configInput(Input::LevelCv2, "Taper 2 level CV");

    Knob::config<Curvature>(this, Param::Curvature2, "Taper 2 curvature");
    Knob::config<Attenuverter>(this, Param::CurvatureAv2,
                               "Taper 2 curvature CV gain");
    Switch::config<Shapes>(this, Param::Shape2, "Taper 2 shape", Shape::J);
    configInput(Input::CurvatureCv2, "Taper 2 curvature CV");

    configOutput(Output::Taper2, "Taper 2");
  }

  void process(ProcessArgs const & /*args*/) override {
    outputs[Output::Taper1].setVoltage(tapered(
        rotation(Param::Level1, Input::LevelCv1, Param::LevelAv1),
        shape(Param::Shape1),
        curvature(Param::Curvature1, Input::CurvatureCv1, Param::CurvatureAv1),
        voltage_range(Param::LevelRange1)));

    outputs[Output::Taper2].setVoltage(tapered(
        rotation(Param::Level2, Input::LevelCv2, Param::LevelAv2),
        shape(Param::Shape2),
        curvature(Param::Curvature2, Input::CurvatureCv2, Param::CurvatureAv2),
        voltage_range(Param::LevelRange2)));
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto curvature(int knob_id, int cv_id, int av_id) const -> float {
    return Curvature::curvature(rotation(knob_id, cv_id, av_id));
  }

  inline auto rotation(int knob_id, int cv_id, int av_id) const -> float {
    return rotation_of(params[knob_id], inputs[cv_id], params[av_id]);
  }

  auto shape(int id) const -> Shape {
    return value_of<Shapes::Selection>(params[id]);
  }

  static inline auto tapered(float rotation, Shape shape, float curvature,
                             VoltageRange range) -> float {
    auto const taper = Shapes::taper(shape);
    auto const tapered = taper.apply(rotation, curvature);
    return VoltageRanges::volts(tapered, range);
  }

  auto voltage_range(int id) const -> VoltageRange {
    return value_of<VoltageRange>(params[id]);
  }
};

} // namespace tapers
} // namespace dhe
