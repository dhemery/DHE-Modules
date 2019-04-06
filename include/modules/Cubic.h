#pragma once

#include <engine.hpp>

namespace DHE {

class Cubic : public rack::Module {
public:
  Cubic();

  void step() override;

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
  auto coefficient(ParameterIds knob_param, InputIds cv_param) const -> float;
  auto gain(ParameterIds knob_param, InputIds cv_input) const -> float;
  auto main_in() const -> float;
  auto modulated(ParameterIds knob_param, InputIds cv_input) const -> float;
  void send_main_out(float voltage);
};
} // namespace DHE
