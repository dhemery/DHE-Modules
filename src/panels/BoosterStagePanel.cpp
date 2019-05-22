#include "panels/BoosterStagePanel.h"

namespace DHE {

BoosterStagePanel::BoosterStagePanel(BoosterStage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = widget_right_edge / 6.f + 0.3333333f;
  auto column_3 = widget_right_edge / 2.f;
  auto column_5 = widget_right_edge - column_1;
  auto button_port_distance = 7.891f;
  auto column_2 = column_1 + button_port_distance;
  auto column_4 = column_5 - button_port_distance;

  auto y = 25.f;
  auto dy = 18.5f;

  input(column_1, y, BoosterStage::LEVEL_CV);
  knob<LargeKnob>(column_3, y, BoosterStage::LEVEL_KNOB);

  auto const setModuleLevelRange = [module](int switchPosition) {
    module->setLevelRange(switchPosition);
  };

  auto levelRangeSwitch = toggle<AnnouncingToggle, 2>(
      column_5, y, BoosterStage::LEVEL_RANGE_SWITCH);
  levelRangeSwitch->announce = setModuleLevelRange;

  y += dy;
  input(column_1, y, BoosterStage::CURVE_CV);
  knob<LargeKnob>(column_3, y, BoosterStage::CURVE_KNOB);
  toggle<2>(column_5, y, BoosterStage::SHAPE_SWITCH);

  y += dy;
  input(column_1, y, BoosterStage::DURATION_CV);
  knob<LargeKnob>(column_3, y, BoosterStage::DURATION_KNOB);

  auto const setModuleDurationRange = [module](int switchPosition) {
    module->setDurationRange(switchPosition);
  };

  auto durationRangeSwitch = toggle<AnnouncingToggle, 3>(
      column_5, y, BoosterStage::DURATION_RANGE_SWITCH);
  durationRangeSwitch->announce = setModuleDurationRange;

  y = 82.f;
  dy = 15.f;

  input(column_1, y, BoosterStage::DEFER_GATE_IN);
  button(column_2, y, BoosterStage::DEFER_BUTTON);

  button<ReverseButton>(column_4, y, BoosterStage::ACTIVE_BUTTON);
  output(column_5, y, BoosterStage::ACTIVE_OUT);

  y += dy;
  input(column_1, y, BoosterStage::STAGE_TRIGGER_IN);
  button(column_2, y, BoosterStage::TRIGGER_BUTTON);
  button<ReverseButton>(column_4, y, BoosterStage::EOC_BUTTON);
  output(column_5, y, BoosterStage::EOC_OUT);

  y += dy;
  input(column_1, y, BoosterStage::ENVELOPE_IN);
  output(column_5, y, BoosterStage::MAIN_OUT);
}
} // namespace DHE
