#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "signals/curvature.h"

#include "rack.hpp"

namespace dhe {
namespace swave {

struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    configInput(InputId::Swave, "Swave");
    configOutput(OutputId::Swave, "Swave");

    Knob::config<Curvature>(this, ParamId::Curvature, "Curvature");
    Knob::config<Attenuverter>(this, ParamId::CurvatureAv, "Curvature CV gain",
                               0.F);
    configInput(InputId::CurvatureCv, "Curvature CV");
    Switch::config<Shapes>(this, ParamId::Shape, "Shape", Shape::J);
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const normalized = BipolarVoltage::normalize(signal_in());
    auto const tapered = Shapes::taper(normalized, shape(), curvature());
    auto const output_voltage = BipolarVoltage::range().scale(tapered);
    send_signal(output_voltage);
  }
  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto curvature() const -> float {
    return rotation_of(params[ParamId::Curvature], inputs[InputId::CurvatureCv],
                       params[ParamId::CurvatureAv]);
  }
  void send_signal(float voltage) {
    outputs[OutputId::Swave].setVoltage(voltage);
  }

  auto shape() const -> Shape {
    return value_of<Shapes::Selection>(params[ParamId::Shape]);
  }

  auto signal_in() const -> float { return voltage_at(inputs[InputId::Swave]); }
};

} // namespace swave
} // namespace dhe
