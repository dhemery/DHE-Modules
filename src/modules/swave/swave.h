#pragma once

#include "config/curvature-config.h"
#include "controls/curvature-inputs.h"
#include "swave-controls.h"

#include <engine/Module.hpp>

namespace dhe {
namespace swave {

class Swave : public rack::engine::Module {
  using Controls = swave;

public:
  Swave() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);
    config_curvature_knob(this, Controls::CurveKnob);
    config_attenuverter(this, Controls::CurveAvKnob, "Curvature CV gain");
    config_curve_shape_switch(this, Controls::ShapeSwitch);
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const normalized = bipolar_signal_range.normalize(signal_in());
    auto const tapered = taper(normalized);
    auto const output_voltage = bipolar_signal_range.scale(tapered);
    send_signal(output_voltage);
  }

private:
  void send_signal(float voltage) {
    outputs[Controls::SwaveOutput].setVoltage(voltage);
  }

  auto signal_in() const -> float {
    return voltage_at(inputs[Controls::SwaveInput]);
  }

  auto taper(float input) const -> float {
    auto const *const taper = selected_taper(params[Controls::ShapeSwitch]);
    return taper->apply(input, curvature(params[Controls::CurveKnob],
                                         inputs[Controls::CurveCvInput],
                                         params[Controls::CurveAvKnob]));
  }
};

} // namespace swave
} // namespace dhe
