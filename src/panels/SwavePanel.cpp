#include "panels/SwavePanel.h"

namespace dhe {

SwavePanel::SwavePanel(Swave *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto x = widgetRightEdge / 2.f;

  auto y = 25.f;
  auto dy = 18.5f;

  toggle<2>(x, y, Swave::ShapeSwitch);

  y += dy;
  knob<LargeKnob>(x, y, Swave::CurveKnob);

  y += dy;
  input(x, y, Swave::CurveCv);

  y = 82.f;
  dy = 15.f;

  y += dy;
  input(x, y, Swave::SwaveInput);

  y += dy;
  output(x, y, Swave::SwaveOutput);
}
} // namespace dhe
