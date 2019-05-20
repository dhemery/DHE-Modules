#include "modules/Ranger.h"

namespace DHE {

Ranger::Ranger() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configKnob(LEVEL_KNOB, "Level");
  configCvGain(LEVEL_AV, "Level");

  configKnob(LIMIT_1_KNOB, "Limit 1");
  configCvGain(LIMIT_1_AV, "Limit 1");
  configSignalRange(LIMIT_1_RANGE_SWITCH, "Limit 1");

  configKnob(LIMIT_2_KNOB, "Limit 2");
  configCvGain(LIMIT_2_AV, "Limit 2");
  configSignalRange(LIMIT_2_RANGE_SWITCH, "Limit 2");

  level = std::unique_ptr<ModulatedKnob>(new ModulatedKnob(
      params[LEVEL_KNOB], inputs[LEVEL_CV], params[LEVEL_AV]));
  zeroRotationLevel = std::unique_ptr<LevelControl>(
      new LevelControl(params[LIMIT_1_KNOB], params[LIMIT_1_RANGE_SWITCH],
                       inputs[LIMIT_1_CV], params[LIMIT_1_AV]));
  fullRotationLevel = std::unique_ptr<LevelControl>(
      new LevelControl(params[LIMIT_2_KNOB], params[LIMIT_2_RANGE_SWITCH],
                       inputs[LIMIT_2_CV], params[LIMIT_1_AV]));
}

void Ranger::process(const ProcessArgs &args) {
  auto const levelRotation = level->rotation();
  auto const zeroRotationVoltage = zeroRotationLevel->voltage();
  auto const fullRotationVoltage = fullRotationLevel->voltage();
  auto const outputVoltage =
      scale(levelRotation, zeroRotationVoltage, fullRotationVoltage);
  outputs[MAIN_OUT].setVoltage(outputVoltage);
}

} // namespace DHE
