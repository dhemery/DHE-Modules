#pragma once

#include "modules/controls/Controls.h"
#include "modules/controls/Inputs.h"
#include "util/Range.h"

#include <engine/Module.hpp>

namespace dhe {

class Cubic : public rack::engine::Module {
public:
  Cubic();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    ACoefficientKnob,
    BCoefficientKnob,
    CCoefficientKnob,
    DCoefficientKnob,
    InputGainKnob,
    OutputGainKnob,
    ParameterCount
  };

  enum InputIds {
    CubicInput,
    ACoefficientCvInput,
    BCoefficientCvInput,
    CCoefficientCvInput,
    DCoefficientCvInput,
    InputGainCvInput,
    OutputGainCvInput,
    InputCount
  };

  enum OutputIds { CubicOutput, OutputCount };

private:
  static auto constexpr coefficientRange = Range{-2.F, 2.F};

  auto coefficient(Cubic::ParameterIds knobParam, Cubic::InputIds cvParam) -> float {
    return scaledRotation(this, knobParam, cvParam, coefficientRange);
  }

  auto gain(const Cubic::ParameterIds knobParam, const Cubic::InputIds cvInput) -> float {
    return scaledRotation(this, knobParam, cvInput, gainRange);
  }

  auto mainIn() -> float { return inputs[CubicInput].getVoltage(); }

  void sendMainOut(float voltage) { outputs[CubicOutput].setVoltage(voltage); }
};
} // namespace dhe
