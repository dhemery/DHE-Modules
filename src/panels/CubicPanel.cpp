#include "display/controls.h"
#include "modules/Cubic.h"
#include "panels/CubicPanel.h"

namespace DHE {

CubicPanel::CubicPanel(rack::Module *cubic) : Panel{cubic, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333f;
  auto column_2 = widget_right_edge - column_1;

  auto y = 20.f;
  auto dy = 15.f;

  install(column_1, y, input(Cubic::A_CV));
  install(column_2, y, knob<SmallKnob>(Cubic::A_KNOB));

  y += dy;
  install(column_1, y, input(Cubic::B_CV));
  install(column_2, y, knob<SmallKnob>(Cubic::B_KNOB));

  y += dy;
  install(column_1, y, input(Cubic::C_CV));
  install(column_2, y, knob<SmallKnob>(Cubic::C_KNOB));

  y += dy;
  install(column_1, y, input(Cubic::D_CV));
  install(column_2, y, knob<SmallKnob>(Cubic::D_KNOB));

  y = 82.f;
  install(column_1, y, knob<SmallKnob>(Cubic::INPUT_GAIN_KNOB));
  install(column_2, y, knob<SmallKnob>(Cubic::OUTPUT_GAIN_KNOB));

  y += dy;
  install(column_1, y, input(Cubic::INPUT_GAIN_CV));
  install(column_2, y, input(Cubic::OUTPUT_GAIN_CV));

  y += dy;
  install(column_1, y, input(Cubic::MAIN_IN));
  install(column_2, y, output(Cubic::MAIN_OUT));
}
}