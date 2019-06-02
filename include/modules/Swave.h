#pragma once

#include "modules/controls/CurvatureControls.h"
#include "modules/controls/TaperControls.h"

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

  auto signalIn() -> float { return inputs[SwaveInput].getVoltage(); }

  auto taper(float input) -> float {
    auto const taper = selectedTaper(this, ShapeSwitch);
    return taper->apply(input, curvature(this, CurveKnob, CurveCv));
  }
};

} // namespace dhe
