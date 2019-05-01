#include "panels/RangerPanel.h"

namespace DHE {

RangerPanel::RangerPanel(Ranger *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 3.5f + 0.333333333f;
  auto column_2 = widget_right_edge - column_1;

  auto y = 24.f;
  auto dy = 16.f;
  auto panel_buffer = 4.f;

  knob<MediumKnob>(column_1, y, Ranger::LEVEL_KNOB);
  output(column_2, y, Ranger::MAIN_OUT);

  y += dy;
  input(column_1, y, Ranger::LEVEL_CV);
  knob<TinyKnob>(column_2, y, Ranger::LEVEL_AV);

  y += dy + panel_buffer;
  knob<MediumKnob>(column_1, y, Ranger::LIMIT_1_KNOB);
  toggle<2>(column_2, y, Ranger::LIMIT_1_RANGE_SWITCH);

  y += dy;
  input(column_1, y, Ranger::LIMIT_1_CV);
  knob<TinyKnob>(column_2, y, Ranger::LIMIT_1_AV);

  y += dy + panel_buffer;
  knob<MediumKnob>(column_1, y, Ranger::LIMIT_2_KNOB);
  toggle<2>(column_2, y, Ranger::LIMIT_2_RANGE_SWITCH);

  y += dy;
  input(column_1, y, Ranger::LIMIT_2_CV);
  knob<TinyKnob>(column_2, y, Ranger::LIMIT_2_AV);
}
} // namespace DHE
