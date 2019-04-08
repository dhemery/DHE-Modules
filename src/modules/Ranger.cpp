#include <modules/Ranger.h>
#include "util/rotation.h"
#include "util/signal.h"

namespace DHE {

Ranger::Ranger() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

void Ranger::step() { send_main_out(scale(level(), limit2(), limit1())); }

auto Ranger::level() const -> float {
  return modulated(LEVEL_KNOB, LEVEL_CV, LEVEL_AV);
}

auto Ranger::limit1() const -> float {
  return limit(LIMIT_1_KNOB, LIMIT_1_CV, LIMIT_1_AV, LIMIT_1_RANGE_SWITCH);
}

auto Ranger::limit2() const -> float {
  return limit(LIMIT_2_KNOB, LIMIT_2_CV, LIMIT_2_AV, LIMIT_2_RANGE_SWITCH);
}

auto Ranger::modulated(Ranger::ParameterIds knob_param,
                       Ranger::InputIds cv_input,
                       Ranger::ParameterIds av_parm) const -> float {
  auto rotation = params[knob_param].value;
  auto cv = inputs[cv_input].value;
  auto av = params[av_parm].value;
  return Rotation::modulated(rotation, cv, av);
}

void Ranger::send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

auto Ranger::limit(Ranger::ParameterIds knob_param, Ranger::InputIds cv_input,
                   Ranger::ParameterIds av_param, int range_switch_param) const
    -> float {
  auto is_uni = params[range_switch_param].value > 0.5f;
  auto range = Signal::range(is_uni);
  return range.scale(modulated(knob_param, cv_input, av_param));
}

} // namespace DHE