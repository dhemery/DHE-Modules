#pragma once

#include "CommonInputs.h"
#include "Range.h"

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
  static Range const coefficientRange;

  auto coefficient(Cubic::ParameterIds knobParam, Cubic::InputIds cvParam) const -> float {
    return coefficientRange.scale(rotation(params[knobParam], inputs[cvParam]));
  }

  auto gain(const Cubic::ParameterIds knobParam, const Cubic::InputIds cvInput) const -> float {
    return gainRange.scale(rotation(params[knobParam], inputs[cvInput]));
  }

  auto mainIn() const -> float { return inputVoltage(inputs[CubicInput]); }

  void sendMainOut(float voltage) { outputs[CubicOutput].setVoltage(voltage); }
};
} // namespace dhe
