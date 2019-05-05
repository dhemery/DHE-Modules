#include "modules/Ranger.h"
#include "util/signal.h"

namespace DHE {

Ranger::Ranger() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configKnob(LEVEL_KNOB, "Level");
  configCvGain(LEVEL_AV, "Level");

  configKnob(LIMIT_1_KNOB, "Limit 1");
  configCvGain(LIMIT_1_AV, "Limit 1");
  configSignalRange(LIMIT_1_RANGE_SWITCH, "Limit 1", false);

  configKnob(LIMIT_2_KNOB, "Limit 2");
  configCvGain(LIMIT_2_AV, "Limit 2");
  configSignalRange(LIMIT_2_RANGE_SWITCH, "Limit 2", false);
}

auto Ranger::level() -> float {
  return modulated(LEVEL_KNOB, LEVEL_CV, LEVEL_AV);
}

auto Ranger::limit(int knob_param, int cv_input, int av_param,
                   int range_switch_param) -> float {
  auto is_uni = params[range_switch_param].value > 0.5f;
  auto range = Signal::range(is_uni);
  return range.scale(modulated(knob_param, cv_input, av_param));
}

auto Ranger::limit1()  -> float {
  return limit(LIMIT_1_KNOB, LIMIT_1_CV, LIMIT_1_AV, LIMIT_1_RANGE_SWITCH);
}

auto Ranger::limit2()  -> float {
  return limit(LIMIT_2_KNOB, LIMIT_2_CV, LIMIT_2_AV, LIMIT_2_RANGE_SWITCH);
}

void Ranger::send_main_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

void Ranger::process(const ProcessArgs &args) {
  send_main_out(scale(level(), limit2(), limit1()));
}

} // namespace DHE
