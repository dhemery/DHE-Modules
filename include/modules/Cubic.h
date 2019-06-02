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
  static Range const coefficientRange;

  auto coefficient(Cubic::ParameterIds knobParam, Cubic::InputIds cvParam) const -> float {
    return scaledRotation(this, knobParam, cvParam, coefficientRange);
  }

  auto gain(const Cubic::ParameterIds knobParam, const Cubic::InputIds cvInput) const -> float {
    return scaledRotation(this, knobParam, cvInput, gainRange);
  }

  auto mainIn() const -> float { return inputVoltage(this, CubicInput); }

  void sendMainOut(float voltage) { outputs[CubicOutput].setVoltage(voltage); }
};
} // namespace dhe
