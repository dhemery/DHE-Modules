#include "panels/RangerPanel.h"

namespace dhe {

RangerPanel::RangerPanel(Ranger *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 3.5f + 0.333333333f;
  auto column_2 = widget_right_edge - column_1;

  auto y = 24.f;
  auto dy = 16.f;
  auto panel_buffer = 4.f;

  knob<MediumKnob>(column_1, y, Ranger::LevelKnob);
  output(column_2, y, Ranger::RangerOutput);

  y += dy;
  input(column_1, y, Ranger::LevelCvInput);
  knob<TinyKnob>(column_2, y, Ranger::LevelAvKnob);

  y += dy + panel_buffer;
  knob<MediumKnob>(column_1, y, Ranger::CcwLimitKnob);
  toggle<2>(column_2, y, Ranger::CcwLimitRangeSwitch);

  y += dy;
  input(column_1, y, Ranger::CcwLimitCvInput);
  knob<TinyKnob>(column_2, y, Ranger::CcwLimitAvKnob);

  y += dy + panel_buffer;
  knob<MediumKnob>(column_1, y, Ranger::CwLimitKnob);
  toggle<2>(column_2, y, Ranger::CwLimitRangeSwitch);

  y += dy;
  input(column_1, y, Ranger::CwLimitCvInput);
  knob<TinyKnob>(column_2, y, Ranger::CwLimitAvKnob);
}
} // namespace dhe
