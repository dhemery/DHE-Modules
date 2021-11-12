#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "signals/curvature.h"

#include "rack.hpp"

namespace dhe {
namespace swave {

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    configInput(Input::Swave, "Swave");
    configOutput(Output::Swave, "Swave");

    Knob::config<Curvature>(this, Param::Curvature, "Curvature");
    Knob::config<Attenuverter>(this, Param::CurvatureAv, "Curvature CV gain",
                               0.F);
    configInput(Input::CurvatureCv, "Curvature CV");
    Switch::config<Shapes>(this, Param::Shape, "Shape", Shape::J);
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
    return rotation_of(params[Param::Curvature], inputs[Input::CurvatureCv],
                       params[Param::CurvatureAv]);
  }
  void send_signal(float voltage) {
    outputs[Output::Swave].setVoltage(voltage);
  }

  auto shape() const -> Shape {
    return value_of<Shapes::Selection>(params[Param::Shape]);
  }

  auto signal_in() const -> float { return voltage_at(inputs[Input::Swave]); }
};

} // namespace swave
} // namespace dhe
