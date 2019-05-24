#include "panels/StagePanel.h"

namespace dhe {
StagePanel::StagePanel(Stage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 25.f;
  auto dy = 18.5f;

  knob<LargeKnob>(column_2, y, Stage::LevelKnob);

  y += dy;
  knob<LargeKnob>(column_2, y, Stage::CurveKnob);

  y += dy;
  knob<LargeKnob>(column_2, y, Stage::DurationKnob);

  y = 82.f;
  dy = 15.f;

  input(column_1, y, Stage::DeferGateInput);
  output(column_3, y, Stage::ActiveOutput);

  y += dy;
  input(column_1, y, Stage::TriggerInput);
  output(column_3, y, Stage::EocOutput);

  y += dy;
  input(column_1, y, Stage::EnvelopeInput);
  output(column_3, y, Stage::EnvelopeOutput);
}
} // namespace dhe
