#pragma once

#include "Module.h"

namespace DHE {

class Cubic : public Module {
public:
  Cubic();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    A_KNOB,
    B_KNOB,
    C_KNOB,
    D_KNOB,
    INPUT_GAIN_KNOB,
    OUTPUT_GAIN_KNOB,
    PARAMETER_COUNT
  };

  enum InputIds {
    MAIN_IN,
    A_CV,
    B_CV,
    C_CV,
    D_CV,
    INPUT_GAIN_CV,
    OUTPUT_GAIN_CV,
    INPUT_COUNT
  };

  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

private:
  auto coefficient(ParameterIds knob_param, InputIds cv_param)  -> float;
  auto gain(ParameterIds knob_param, InputIds cv_input)  -> float;
  auto main_in()  -> float;
  void send_main_out(float voltage);
};
} // namespace DHE
