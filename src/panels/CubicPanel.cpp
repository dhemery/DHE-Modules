#include "panels/CubicPanel.h"

namespace dhe {

CubicPanel::CubicPanel(Cubic *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333f;
  auto column_2 = widget_right_edge - column_1;

  auto y = 20.f;
  auto dy = 15.f;

  input(column_1, y, Cubic::A_CV);
  knob<SmallKnob>(column_2, y, Cubic::A_KNOB);

  y += dy;
  input(column_1, y, Cubic::B_CV);
  knob<SmallKnob>(column_2, y, Cubic::B_KNOB);

  y += dy;
  input(column_1, y, Cubic::C_CV);
  knob<SmallKnob>(column_2, y, Cubic::C_KNOB);

  y += dy;
  input(column_1, y, Cubic::D_CV);
  knob<SmallKnob>(column_2, y, Cubic::D_KNOB);

  y = 82.f;
  knob<SmallKnob>(column_1, y, Cubic::INPUT_GAIN_KNOB);
  knob<SmallKnob>(column_2, y, Cubic::OUTPUT_GAIN_KNOB);

  y += dy;
  input(column_1, y, Cubic::INPUT_GAIN_CV);
  input(column_2, y, Cubic::OUTPUT_GAIN_CV);

  y += dy;
  input(column_1, y, Cubic::MAIN_IN);
  output(column_2, y, Cubic::MAIN_OUT);
}
} // namespace dhe
