#include "panels/StagePanel.h"

namespace dhe {
StagePanel::StagePanel(Stage *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 4.f + 0.333333f;
  auto column2 = widgetRightEdge / 2.f;
  auto column3 = widgetRightEdge - column1;

  auto y = 25.f;
  auto dy = 18.5f;

  knob<LargeKnob>(column2, y, Stage::LevelKnob);

  y += dy;
  knob<LargeKnob>(column2, y, Stage::CurveKnob);

  y += dy;
  knob<LargeKnob>(column2, y, Stage::DurationKnob);

  y = 82.f;
  dy = 15.f;

  input(column1, y, Stage::DeferGateInput);
  output(column3, y, Stage::ActiveOutput);

  y += dy;
  input(column1, y, Stage::TriggerInput);
  output(column3, y, Stage::EocOutput);

  y += dy;
  input(column1, y, Stage::EnvelopeInput);
  output(column3, y, Stage::EnvelopeOutput);
}
} // namespace dhe
