#pragma once

#include "control-ids.h"
#include "controls/knobs.h"
#include "controls/shape-controls.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/curvature-signals.h"
#include "signals/gain.h"
#include "signals/shapes.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {
namespace swave {

struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    configInput(InputId::Swave, "Swave");
    configOutput(OutputId::Swave, "Swave");

    CurvatureKnob::config(this, ParamId::Curvature, "Curvature");
    Knob::config<Attenuverter>(this, ParamId::CurvatureAv, "Curvature CV gain",
                               0.F);
    configInput(InputId::CurvatureCv, "Curvature CV");
    Switch::config<Shapes>(this, ParamId::Shape, "Shape", Shape::Id::J);
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const normalized = BipolarVoltage::normalize(input_voltage());
    auto const tapered = Shape::apply(normalized, shape(), curvature());
    auto const output_voltage = BipolarVoltage::scale(tapered);
    send_signal(output_voltage);
  }
  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto curvature() const -> float {
    auto const rotation =
        rotation_of(params[ParamId::Curvature], inputs[InputId::CurvatureCv],
                    params[ParamId::CurvatureAv]);
    return Curvature::scale(Rotation::clamp(rotation));
  }

  void send_signal(float voltage) {
    outputs[OutputId::Swave].setVoltage(voltage);
  }

  auto shape() const -> Shape::Id {
    return value_of<Shape::Id>(params[ParamId::Shape]);
  }

  auto input_voltage() const -> float {
    return BipolarVoltage::clamp(voltage_at(inputs[InputId::Swave]));
  }
};

} // namespace swave
} // namespace dhe
