#include <panels/UpstagePanel.h>

namespace DHE {

UpstagePanel::UpstagePanel(Upstage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 25.f;
  auto dy = 18.5f;

  install(column_2, y, knob<LargeKnob>(Upstage::LEVEL_KNOB));

  y += dy;
  install(column_1, y, input(Upstage::LEVEL_CV));
  install(column_3, y,
          toggle<2>(Upstage::LEVEL_RANGE_SWITCH, 1,
                    module->level_range_selector));

  y += dy;
  install(column_1, y, button(Upstage::WAIT_BUTTON));
  install(column_3, y, button(Upstage::TRIGGER_BUTTON));

  y = 82.f;
  dy = 15.f;

  install(column_1, y, input(Upstage::WAIT_IN));

  y += dy;
  install(column_1, y, input(Upstage::TRIGGER_IN));
  install(column_3, y, output(Upstage::TRIGGER_OUT));

  y += dy;
  install(column_3, y, output(Upstage::MAIN_OUT));
}
} // namespace DHE
