#include "panels/BoosterStagePanel.h"

namespace dhe {

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

  input(column_1, y, BoosterStage::LevelCvInput);
  knob<LargeKnob>(column_3, y, BoosterStage::LevelKnob);

  toggle<2>(column_5, y, BoosterStage::LevelRangeSwitch);

  y += dy;
  input(column_1, y, BoosterStage::CurveCvInput);
  knob<LargeKnob>(column_3, y, BoosterStage::CurveKnob);
  toggle<2>(column_5, y, BoosterStage::ShapeSwitch);

  y += dy;
  input(column_1, y, BoosterStage::DurationCvInput);
  knob<LargeKnob>(column_3, y, BoosterStage::DurationKnob);

  toggle<3>(column_5, y, BoosterStage::DurationRangeSwitch);

  y = 82.f;
  dy = 15.f;

  input(column_1, y, BoosterStage::DeferGateInput);
  button(column_2, y, BoosterStage::DeferButton);

  button<ReverseButton>(column_4, y, BoosterStage::ActiveButton);
  output(column_5, y, BoosterStage::ActiveOutput);

  y += dy;
  input(column_1, y, BoosterStage::TriggerInput);
  button(column_2, y, BoosterStage::TriggerButton);
  button<ReverseButton>(column_4, y, BoosterStage::EocButton);
  output(column_5, y, BoosterStage::EocOutput);

  y += dy;
  input(column_1, y, BoosterStage::EnvelopeInput);
  output(column_5, y, BoosterStage::EnvelopeOutput);
}
} // namespace dhe
