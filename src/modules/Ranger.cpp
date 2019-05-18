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

  limit1 = std::unique_ptr<LevelControl>(
      new LevelControl(params[LIMIT_1_KNOB], params[LIMIT_1_RANGE_SWITCH],
                       inputs[LIMIT_1_CV], params[LIMIT_1_AV]));
  limit2 = std::unique_ptr<LevelControl>(
      new LevelControl(params[LIMIT_2_KNOB], params[LIMIT_2_RANGE_SWITCH],
                       inputs[LIMIT_2_CV], params[LIMIT_1_AV]));
}

auto Ranger::level() -> float {
  return modulated(LEVEL_KNOB, LEVEL_CV, LEVEL_AV);
}

void Ranger::send_main_out(float voltage) {
  outputs[MAIN_OUT].setVoltage(voltage);
}

void Ranger::process(const ProcessArgs &args) {
  send_main_out(scale(level(), limit1->voltage(), limit2->voltage()));
}

} // namespace DHE
