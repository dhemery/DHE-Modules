#pragma once

#include "engine.hpp"

namespace DHE {

class Swave : public rack::Module {
public:
  Swave();
  void step() override;

  enum ParameterIds { CURVE_KNOB, SHAPE_SWITCH, PARAMETER_COUNT };
  enum InputIds { CURVE_CV, MAIN_IN, INPUT_COUNT };
  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

private:
  auto curve() const -> float;
  void send_signal(float voltage);
  auto shape() const -> float;
  auto signal_in() const -> float;
};

} // namespace DHE
