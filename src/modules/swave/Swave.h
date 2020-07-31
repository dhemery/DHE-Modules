#pragma once

#include "SwaveControls.h"
#include "config/CurvatureConfig.h"
#include "controls/CurvatureInputs.h"

#include <engine/Module.hpp>

namespace dhe {
namespace swave {

  class Swave : public rack::engine::Module {
    using Controls = SwaveControls;

  public:
    Swave() {
      config(Controls::ParameterCount, Controls::InputCount, Controls::OutputCount);
      configCurvatureKnob(this, Controls::CurveKnob);
      configAttenuverter(this, Controls::CurveAvKnob, "Curvature CV gain");
      configCurveShapeSwitch(this, Controls::ShapeSwitch);
    }

    void process(ProcessArgs const & /*args*/) override {
      auto const normalized = bipolarSignalRange.normalize(signalIn());
      auto const tapered = taper(normalized);
      auto const outputVoltage = bipolarSignalRange.scale(tapered);
      sendSignal(outputVoltage);
    }

  private:
    void sendSignal(float voltage) { outputs[Controls::SwaveOutput].setVoltage(voltage); }

    auto signalIn() const -> float { return voltageAt(inputs[Controls::SwaveInput]); }

    auto taper(float input) const -> float {
      auto const *const taper = selectedTaper(params[Controls::ShapeSwitch]);
      return taper->apply(
          input, curvature(params[Controls::CurveKnob], inputs[Controls::CurveCvInput], params[Controls::CurveAvKnob]));
    }
  };

} // namespace swave
} // namespace dhe
