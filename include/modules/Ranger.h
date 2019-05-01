#pragma once

#include "Module.h"

namespace DHE {

class Ranger : public Module {
public:
  Ranger();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    LEVEL_KNOB,
    LEVEL_AV,
    LIMIT_1_KNOB,
    LIMIT_1_AV,
    LIMIT_1_RANGE_SWITCH,
    LIMIT_2_KNOB,
    LIMIT_2_AV,
    LIMIT_2_RANGE_SWITCH,
    PARAMETER_COUNT
  };
  enum InputIds { LEVEL_CV, LIMIT_1_CV, LIMIT_2_CV, INPUT_COUNT };
  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

private:
  auto level() const -> float;
  auto limit(int knob_param, int cv_input, int av_param,
             int range_switch_param) const -> float;
  auto limit1() const -> float;
  auto limit2() const -> float;
  void send_main_out(float voltage);
};

} // namespace DHE
