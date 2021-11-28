#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "controls/knobs.h"
#include "controls/shape-controls.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/linear-signals.h"
#include "signals/shape-signals.h"
#include "signals/voltage-signals.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {

class Module : public rack::engine::Module {
public:
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    auto *level_knob_1 =
        VoltageKnob::config(this, ParamId::Level1, "Taper 1 level");
    VoltageRangeSwitch::config(this, ParamId::LevelRange1,
                               "Taper 1 level range", VoltageRangeId::Bipolar)
        ->add_knob(level_knob_1);
    AttenuverterKnob::config(this, ParamId::LevelAv1, "Taper 1 level CV gain");
    configInput(InputId::LevelCv1, "Taper 1 level CV");

    CurvatureKnob::config(this, ParamId::Curvature1, "Taper 1 curvature");
    AttenuverterKnob::config(this, ParamId::CurvatureAv1,
                             "Taper 1 curvature CV gain");
    ShapeSwitch::config(this, ParamId::Shape1, "Taper 1 shape", Shape::Id::J);
    configInput(InputId::CurvatureCv1, "Taper 1 curvature CV");

    configOutput(OutputId::Taper1, "Taper 1");

    auto *level_knob_2 =
        VoltageKnob::config(this, ParamId::Level2, "Taper 2 level");
    VoltageRangeSwitch::config(this, ParamId::LevelRange2,
                               "Taper 2 level range", VoltageRangeId::Bipolar)
        ->add_knob(level_knob_2);
    AttenuverterKnob::config(this, ParamId::LevelAv2, "Taper 2 level CV gain");
    configInput(InputId::LevelCv2, "Taper 2 level CV");

    CurvatureKnob::config(this, ParamId::Curvature2, "Taper 2 curvature");
    AttenuverterKnob::config(this, ParamId::CurvatureAv2,
                             "Taper 2 curvature CV gain");
    ShapeSwitch::config(this, ParamId::Shape2, "Taper 2 shape", Shape::Id::J);
    configInput(InputId::CurvatureCv2, "Taper 2 curvature CV");

    configOutput(OutputId::Taper2, "Taper 2Shape");
  }

  void process(ProcessArgs const & /*args*/) override {
    outputs[OutputId::Taper1].setVoltage(tapered(
        safe_rotation(ParamId::Level1, InputId::LevelCv1, ParamId::LevelAv1),
        shape(ParamId::Shape1),
        curvature(ParamId::Curvature1, InputId::CurvatureCv1,
                  ParamId::CurvatureAv1),
        voltage_range(ParamId::LevelRange1)));

    outputs[OutputId::Taper2].setVoltage(tapered(
        safe_rotation(ParamId::Level2, InputId::LevelCv2, ParamId::LevelAv2),
        shape(ParamId::Shape2),
        curvature(ParamId::Curvature2, InputId::CurvatureCv2,
                  ParamId::CurvatureAv2),
        voltage_range(ParamId::LevelRange2)));
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  inline auto safe_rotation(int knob_id, int cv_id, int av_id) const -> float {
    return Rotation::clamp(
        rotation_of(params[knob_id], inputs[cv_id], params[av_id]));
  }

  inline auto curvature(int knob_id, int cv_id, int av_id) -> float {
    return Curvature::scale(safe_rotation(knob_id, cv_id, av_id));
  }

  auto shape(int id) const -> Shape::Id {
    return value_of<Shape::Id>(params[id]);
  }

  static inline auto tapered(float rotation, Shape::Id shape, float curvature,
                             VoltageRangeId range_id) -> float {
    auto const tapered = Shape::apply(rotation, shape, curvature);
    return Voltage::scale(tapered, range_id);
  }

  auto voltage_range(int id) const -> VoltageRangeId {
    return value_of<VoltageRangeId>(params[id]);
  }
};

} // namespace tapers
} // namespace dhe
