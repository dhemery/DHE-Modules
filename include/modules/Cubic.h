#pragma once

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
  auto coefficient(ParameterIds knobParam, InputIds cvParam) -> float;
  auto gain(ParameterIds knobParam, InputIds cvInput) -> float;
  auto mainIn() -> float;
  void sendMainOut(float voltage);
};
} // namespace dhe
