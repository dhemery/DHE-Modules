#pragma once

#include "Module.h"

namespace DHE {

class Swave : public Module {
public:
  Swave();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { CURVE_KNOB, SHAPE_SWITCH, PARAMETER_COUNT };
  enum InputIds { CURVE_CV, MAIN_IN, INPUT_COUNT };
  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

private:
  auto curve() const -> float;
  auto is_s() const -> bool;
  void send_signal(float voltage);
  auto signal_in() const -> float;
};

} // namespace DHE
