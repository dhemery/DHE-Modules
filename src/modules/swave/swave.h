#pragma once

#include "./swave-controls.h"
#include "config/curvature-config.h"
#include "controls/curvature-inputs.h"

#include <rack.hpp>

namespace dhe {
namespace swave {

class Swave : public rack::engine::Module {
  using Controls = SwaveControls;

public:
  Swave() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    configInput(Controls::SwaveInput, "Swave");
    configOutput(Controls::SwaveOutput, "Swave");

    config_curvature_knob(this, Controls::CurveKnob);
    config_attenuverter(this, Controls::CurveAvKnob, "Curvature CV gain");
    configInput(Controls::CurveCvInput, "Curvature CV");
    config_curve_shape_switch(this, Controls::ShapeSwitch);
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
    outputs[Controls::SwaveOutput].setVoltage(voltage);
  }

  auto signal_in() const -> float {
    return voltage_at(inputs[Controls::SwaveInput]);
  }

  auto taper(float input) const -> float {
    auto const &taper = selected_taper(params[Controls::ShapeSwitch]);
    return taper.apply(input, curvature(params[Controls::CurveKnob],
                                        inputs[Controls::CurveCvInput],
                                        params[Controls::CurveAvKnob]));
  }
};

} // namespace swave
} // namespace dhe
