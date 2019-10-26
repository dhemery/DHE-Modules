#include "module/stage/StagePanel.h"

namespace dhe {
StagePanel::StagePanel(Stage *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 4.F + 0.333333F;
  auto column2 = widgetRightEdge / 2.F;
  auto column3 = widgetRightEdge - column1;

  auto y = 25.F;
  auto dy = 18.5F;

  knob<LargeKnob>(column2, y, Stage::LevelKnob);

  y += dy;
  knob<LargeKnob>(column2, y, Stage::CurveKnob);

  y += dy;
  knob<LargeKnob>(column2, y, Stage::DurationKnob);

  y = 82.F;
  dy = 15.F;

  input(column1, y, Stage::DeferInput);
  output(column3, y, Stage::ActiveOutput);

  y += dy;
  input(column1, y, Stage::TriggerInput);
  output(column3, y, Stage::EocOutput);

  y += dy;
  input(column1, y, Stage::EnvelopeInput);
  output(column3, y, Stage::EnvelopeOutput);
}
} // namespace dhe
