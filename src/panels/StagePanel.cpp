#include <panels/StagePanel.h>

namespace DHE {
StagePanel::StagePanel(Stage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 25.f;
  auto dy = 18.5f;

  install(column_2, y, knob<LargeKnob>(Stage::LEVEL_KNOB));

  y += dy;
  install(column_2, y, knob<LargeKnob>(Stage::CURVE_KNOB));

  y += dy;
  install(column_2, y, knob<LargeKnob>(Stage::DURATION_KNOB));

  y = 82.f;
  dy = 15.f;

  install(column_1, y, input(Stage::DEFER_GATE_IN));
  install(column_3, y, output(Stage::ACTIVE_OUT));

  y += dy;
  install(column_1, y, input(Stage::STAGE_TRIGGER_IN));
  install(column_3, y, output(Stage::EOC_OUT));

  y += dy;
  install(column_1, y, input(Stage::ENVELOPE_IN));
  install(column_3, y, output(Stage::MAIN_OUT));
}
} // namespace DHE
