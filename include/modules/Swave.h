#pragma once

#include "modules/controls/CurvatureInputs.h"

#include <engine/Module.hpp>

namespace dhe {

class Swave : public rack::engine::Module {
public:
  Swave();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { CurveKnob, ShapeSwitch, ParameterCount };
  enum InputIds { CurveCv, SwaveInput, InputCount };
  enum OutputIds { SwaveOutput, OutputCount };

private:
  void sendSignal(float voltage) { outputs[SwaveOutput].setVoltage(voltage); }

  auto signalIn() const -> float { return inputVoltage(this, SwaveInput); }

  auto taper(float input) const -> float {
    auto const taper = selectedTaper(this, ShapeSwitch);
    return taper->apply(input, curvature(this, CurveKnob, CurveCv));
  }
};

} // namespace dhe
