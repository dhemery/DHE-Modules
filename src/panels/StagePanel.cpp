#include "panels/StagePanel.h"

namespace DHE {
StagePanel::StagePanel(Stage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 25.f;
  auto dy = 18.5f;

  knob<LargeKnob>(column_2, y, Stage::LEVEL_KNOB);

  y += dy;
  knob<LargeKnob>(column_2, y, Stage::CURVE_KNOB);

  y += dy;
  knob<LargeKnob>(column_2, y, Stage::DURATION_KNOB);

  y = 82.f;
  dy = 15.f;

  input(column_1, y, Stage::DEFER_GATE_IN);
  output(column_3, y, Stage::ACTIVE_OUT);

  y += dy;
  input(column_1, y, Stage::STAGE_TRIGGER_IN);
  output(column_3, y, Stage::EOC_OUT);

  y += dy;
  input(column_1, y, Stage::ENVELOPE_IN);
  output(column_3, y, Stage::MAIN_OUT);
}
} // namespace DHE
