#pragma once

#include "modules/controls/CurvatureInputs.h"

#include <engine/Module.hpp>

namespace dhe {

class Swave : public rack::engine::Module {
public:
  Swave();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { CurveKnob, ShapeSwitch, CurveAvKnob, ParameterCount };
  enum InputIds { CurveCvInput, SwaveInput, InputCount };
  enum OutputIds { SwaveOutput, OutputCount };

private:
  void sendSignal(float voltage) { outputs[SwaveOutput].setVoltage(voltage); }

  auto signalIn() const -> float { return inputVoltage(inputs[SwaveInput]); }

  auto taper(float input) const -> float {
    auto const taper = selectedTaper(params[ShapeSwitch]);
    return taper->apply(input, curvature(this, CurveKnob, CurveCvInput, CurveAvKnob));
  }
};

} // namespace dhe
