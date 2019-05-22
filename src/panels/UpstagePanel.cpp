#include "panels/UpstagePanel.h"

namespace dhe {

UpstagePanel::UpstagePanel(Upstage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 25.f;
  auto dy = 18.5f;

  knob<LargeKnob>(column_2, y, Upstage::LEVEL_KNOB);

  y += dy;
  input(column_1, y, Upstage::LEVEL_CV);
  toggle<2>(column_3, y, Upstage::LEVEL_RANGE_SWITCH);

  y += dy;
  button(column_1, y, Upstage::WAIT_BUTTON);
  button(column_3, y, Upstage::TRIGGER_BUTTON);

  y = 82.f;
  dy = 15.f;

  input(column_1, y, Upstage::WAIT_IN);

  y += dy;
  input(column_1, y, Upstage::TRIGGER_IN);
  output(column_3, y, Upstage::TRIGGER_OUT);

  y += dy;
  output(column_3, y, Upstage::MAIN_OUT);
}
} // namespace dhe
