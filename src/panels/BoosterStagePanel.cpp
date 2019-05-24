#include "panels/BoosterStagePanel.h"

namespace dhe {

BoosterStagePanel::BoosterStagePanel(BoosterStage *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = widgetRightEdge / 6.f + 0.3333333f;
  auto column3 = widgetRightEdge / 2.f;
  auto column5 = widgetRightEdge - column1;
  auto buttonPortDistance = 7.891f;
  auto column2 = column1 + buttonPortDistance;
  auto column4 = column5 - buttonPortDistance;

  auto y = 25.f;
  auto dy = 18.5f;

  input(column1, y, BoosterStage::LevelCvInput);
  knob<LargeKnob>(column3, y, BoosterStage::LevelKnob);

  toggle<2>(column5, y, BoosterStage::LevelRangeSwitch);

  y += dy;
  input(column1, y, BoosterStage::CurveCvInput);
  knob<LargeKnob>(column3, y, BoosterStage::CurveKnob);
  toggle<2>(column5, y, BoosterStage::ShapeSwitch);

  y += dy;
  input(column1, y, BoosterStage::DurationCvInput);
  knob<LargeKnob>(column3, y, BoosterStage::DurationKnob);

  toggle<3>(column5, y, BoosterStage::DurationRangeSwitch);

  y = 82.f;
  dy = 15.f;

  input(column1, y, BoosterStage::DeferGateInput);
  button(column2, y, BoosterStage::DeferButton);

  button<ReverseButton>(column4, y, BoosterStage::ActiveButton);
  output(column5, y, BoosterStage::ActiveOutput);

  y += dy;
  input(column1, y, BoosterStage::TriggerInput);
  button(column2, y, BoosterStage::TriggerButton);
  button<ReverseButton>(column4, y, BoosterStage::EocButton);
  output(column5, y, BoosterStage::EocOutput);

  y += dy;
  input(column1, y, BoosterStage::EnvelopeInput);
  output(column5, y, BoosterStage::EnvelopeOutput);
}
} // namespace dhe
