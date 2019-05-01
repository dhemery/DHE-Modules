#include "panels/HostagePanel.h"

namespace DHE {

HostagePanel::HostagePanel(Hostage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 25.f;
  auto dy = 18.5f;

  toggle<2>(column_2, y, Hostage::HOSTAGE_MODE_SWITCH);

  y += dy;
  input(column_1, y, Hostage::DURATION_CV);
  toggle<3>(column_3, y, Hostage::DURATION_RANGE_SWITCH);

  y += dy;
  knob<LargeKnob>(column_2, y, Hostage::DURATION_KNOB);

  y = 82.f;
  dy = 15.f;

  input(column_1, y, Hostage::DEFER_GATE_IN);
  output(column_3, y, Hostage::ACTIVE_OUT);

  y += dy;
  input(column_1, y, Hostage::STAGE_GATE_IN);
  output(column_3, y, Hostage::EOC_OUT);

  y += dy;
  input(column_1, y, Hostage::MAIN_IN);
  output(column_3, y, Hostage::MAIN_OUT);
}
} // namespace DHE
