#include <panels/RangerPanel.h>

namespace DHE {

RangerPanel::RangerPanel(Ranger *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 3.5f + 0.333333333f;
  auto column_2 = widget_right_edge - column_1;

  auto y = 24.f;
  auto dy = 16.f;
  auto panel_buffer = 4.f;

  install(column_1, y, knob<MediumKnob>(Ranger::LEVEL_KNOB));
  install(column_2, y, output(Ranger::MAIN_OUT));

  y += dy;
  install(column_1, y, input(Ranger::LEVEL_CV));
  install(column_2, y, knob<TinyKnob>(Ranger::LEVEL_AV));

  y += dy + panel_buffer;
  install(column_1, y, knob<MediumKnob>(Ranger::LIMIT_1_KNOB));
  install(column_2, y, toggle<2>(Ranger::LIMIT_1_RANGE_SWITCH, 1));

  y += dy;
  install(column_1, y, input(Ranger::LIMIT_1_CV));
  install(column_2, y, knob<TinyKnob>(Ranger::LIMIT_1_AV));

  y += dy + panel_buffer;
  install(column_1, y, knob<MediumKnob>(Ranger::LIMIT_2_KNOB));
  install(column_2, y, toggle<2>(Ranger::LIMIT_2_RANGE_SWITCH, 1));

  y += dy;
  install(column_1, y, input(Ranger::LIMIT_2_CV));
  install(column_2, y, knob<TinyKnob>(Ranger::LIMIT_2_AV));
}
} // namespace DHE
