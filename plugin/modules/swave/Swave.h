#pragma once

#include "controls/CurvatureInputs.h"

#include <engine/Module.hpp>

namespace dhe {

class Swave : public rack::engine::Module {
public:
  Swave();
  void process(ProcessArgs const &args) override;

  enum ParameterIds { CurveKnob, ShapeSwitch, CurveAvKnob, ParameterCount };
  enum InputIds { CurveCvInput, SwaveInput, InputCount };
  enum OutputIds { SwaveOutput, OutputCount };

private:
  void sendSignal(float voltage) { outputs[SwaveOutput].setVoltage(voltage); }

  auto signalIn() const -> float { return voltageAt(inputs[SwaveInput]); }

  auto taper(float input) const -> float {
    auto const taper = selectedTaper(params[ShapeSwitch]);
    return taper->apply(input, curvature(params[CurveKnob], inputs[CurveCvInput], params[CurveAvKnob]));
  }
};

} // namespace dhe
