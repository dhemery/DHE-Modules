#include "panels/HostagePanel.h"

namespace DHE {

HostagePanel::HostagePanel(Hostage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 25.f;
  auto dy = 18.5f;

  install(column_2, y, toggle<2>(Hostage::HOSTAGE_MODE_SWITCH, 0));

  y += dy;
  install(column_1, y, input(Hostage::DURATION_CV));
  install(column_3, y,
          toggle<3>(Hostage::DURATION_RANGE_SWITCH, 1,
                    module->duration_range_selector));

  y += dy;
  install(column_2, y, knob<LargeKnob>(Hostage::DURATION_KNOB));

  y = 82.f;
  dy = 15.f;

  install(column_1, y, input(Hostage::DEFER_GATE_IN));
  install(column_3, y, output(Hostage::ACTIVE_OUT));

  y += dy;
  install(column_1, y, input(Hostage::STAGE_GATE_IN));
  install(column_3, y, output(Hostage::EOC_OUT));

  y += dy;
  install(column_1, y, input(Hostage::MAIN_IN));
  install(column_3, y, output(Hostage::MAIN_OUT));
}
} // namespace DHE
