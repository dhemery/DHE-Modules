#pragma once

#include "engine.hpp"

namespace DHE {

class Ranger : public rack::Module {
public:
  Ranger();

  void step() override;

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
  auto limit(ParameterIds knob_param, InputIds cv_input, ParameterIds av_param,
             int range_switch_param) const -> float;
  auto limit1() const -> float;
  auto limit2() const -> float;
  auto modulated(ParameterIds knob_param, InputIds cv_input,
                 ParameterIds av_parm) const -> float;
  void send_main_out(float voltage);
};

} // namespace DHE
