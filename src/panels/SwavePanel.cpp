#include "panels/SwavePanel.h"

namespace DHE {

SwavePanel::SwavePanel(Swave *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto x = widget_right_edge / 2.f;

  auto y = 25.f;
  auto dy = 18.5f;

  install(x, y, toggle<2>(Swave::SHAPE_SWITCH, 1));

  y += dy;
  install(x, y, knob<LargeKnob>(Swave::CURVE_KNOB));

  y += dy;
  install(x, y, input(Swave::CURVE_CV));

  y = 82.f;
  dy = 15.f;

  y += dy;
  install(x, y, input(Swave::MAIN_IN));

  y += dy;
  install(x, y, output(Swave::MAIN_OUT));
}
} // namespace DHE
