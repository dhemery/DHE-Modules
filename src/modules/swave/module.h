#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "params/curvature-config.h"
#include "signals/curvature-inputs.h"

#include "rack.hpp"

namespace dhe {
namespace swave {

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    configInput(Input::Swave, "Swave");
    configOutput(Output::Swave, "Swave");

    config_curvature_knob(this, Param::Curvature);
    Knob::config<Attenuverter>(this, Param::CurvatureAv, "Curvature CV gain",
                               0.F);
    configInput(Input::CurvatureCv, "Curvature CV");
    config_curve_shape_switch(this, Param::Shape);
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const normalized = bipolar_signal_range.normalize(signal_in());
    auto const tapered = taper(normalized);
    auto const output_voltage = bipolar_signal_range.scale(tapered);
    send_signal(output_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  void send_signal(float voltage) {
    outputs[Output::Swave].setVoltage(voltage);
  }

  auto signal_in() const -> float { return voltage_at(inputs[Input::Swave]); }

  auto taper(float input) const -> float {
    auto const &taper = selected_taper(params[Param::Shape]);
    return taper.apply(input, curvature(params[Param::Curvature],
                                        inputs[Input::CurvatureCv],
                                        params[Param::CurvatureAv]));
  }
};

} // namespace swave
} // namespace dhe
