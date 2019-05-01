#include "panels/SwavePanel.h"

namespace DHE {

SwavePanel::SwavePanel(Swave *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto x = widget_right_edge / 2.f;

  auto y = 25.f;
  auto dy = 18.5f;

  toggle<2>(x, y, Swave::SHAPE_SWITCH);

  y += dy;
  knob<LargeKnob>(x, y, Swave::CURVE_KNOB);

  y += dy;
  input(x, y, Swave::CURVE_CV);

  y = 82.f;
  dy = 15.f;

  y += dy;
  input(x, y, Swave::MAIN_IN);

  y += dy;
  output(x, y, Swave::MAIN_OUT);
}
} // namespace DHE
