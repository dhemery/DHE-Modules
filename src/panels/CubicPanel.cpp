#include "panels/CubicPanel.h"

namespace dhe {

CubicPanel::CubicPanel(Cubic *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 4.F + 0.333333f;
  auto column2 = widgetRightEdge - column1;

  auto y = 20.F;
  auto dy = 15.F;

  input(column1, y, Cubic::ACoefficientCvInput);
  knob<SmallKnob>(column2, y, Cubic::ACoefficientKnob);

  y += dy;
  input(column1, y, Cubic::BCoefficientCvInput);
  knob<SmallKnob>(column2, y, Cubic::BCoefficientKnob);

  y += dy;
  input(column1, y, Cubic::CCoefficientCvInput);
  knob<SmallKnob>(column2, y, Cubic::CCoefficientKnob);

  y += dy;
  input(column1, y, Cubic::DCoefficientCvInput);
  knob<SmallKnob>(column2, y, Cubic::DCoefficientKnob);

  y = 82.F;
  knob<SmallKnob>(column1, y, Cubic::InputGainKnob);
  knob<SmallKnob>(column2, y, Cubic::OutputGainKnob);

  y += dy;
  input(column1, y, Cubic::InputGainCvInput);
  input(column2, y, Cubic::OutputGainCvInput);

  y += dy;
  input(column1, y, Cubic::CubicInput);
  output(column2, y, Cubic::CubicOutput);
}
} // namespace dhe
