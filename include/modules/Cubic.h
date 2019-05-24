#pragma once

#include "Module.h"

namespace dhe {

class Cubic : public Module {
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
  auto coefficient(ParameterIds knob_param, InputIds cv_param) -> float;
  auto gain(ParameterIds knob_param, InputIds cv_input) -> float;
  auto main_in() -> float;
  void send_main_out(float voltage);
};
} // namespace dhe
