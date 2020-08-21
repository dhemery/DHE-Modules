#pragma once

#include "SwaveControls.h"
#include "config/curvature-config.h"
#include "controls/curvature-inputs.h"

#include <engine/Module.hpp>

namespace dhe {
namespace swave {

class Swave : public rack::engine::Module {
  using Controls = SwaveControls;

public:
  Swave() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);
    config_curvature_knob(this, Controls::CurveKnob);
    config_attenuverter(this, Controls::CurveAvKnob, "Curvature CV gain");
    config_curve_shape_switch(this, Controls::ShapeSwitch);
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const normalized = bipolar_signal_range.normalize(signalIn());
    auto const tapered = taper(normalized);
    auto const outputVoltage = bipolar_signal_range.scale(tapered);
    sendSignal(outputVoltage);
  }

private:
  void sendSignal(float voltage) {
    outputs[Controls::SwaveOutput].setVoltage(voltage);
  }

  auto signalIn() const -> float {
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
