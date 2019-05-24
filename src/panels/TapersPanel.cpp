#include "panels/TapersPanel.h"

namespace dhe {

TapersPanel::TapersPanel(Tapers *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 5.F + 0.333333333F;
  auto column2 = widgetRightEdge / 2.F;
  auto column3 = widgetRightEdge - column1;

  auto y = 24.F;
  auto dy = 16.F;
  auto panelBuffer = 4.F;

  input(column1, y, Tapers::Level1Cv);
  knob<TinyKnob>(column2, y, Tapers::LevelAvKnob1);
  knob<MediumKnob>(column3, y, Tapers::LevelKnob1);
  y += dy;
  input(column1, y, Tapers::Curve1Cv);
  knob<TinyKnob>(column2, y, Tapers::CurveAvKnob1);
  knob<MediumKnob>(column3, y, Tapers::CurveKnob1);
  y += dy;
  toggle<2>(column1, y, Tapers::ShapeSwitch1);
  toggle<2>(column2, y, Tapers::LevelRangeSwitch1);
  output(column3, y, Tapers::Taper1Output);

  y += dy + panelBuffer;

  input(column1, y, Tapers::Level2Cv);
  knob<TinyKnob>(column2, y, Tapers::LevelAvKnob2);
  knob<MediumKnob>(column3, y, Tapers::LevelKnob2);
  y += dy;
  input(column1, y, Tapers::Curve2Cv);
  knob<TinyKnob>(column2, y, Tapers::Curve2Av);
  knob<MediumKnob>(column3, y, Tapers::Curve2Knob);
  y += dy;
  toggle<2>(column1, y, Tapers::Shape2Switch);
  toggle<2>(column2, y, Tapers::LevelRangeSwitch2);
  output(column3, y, Tapers::Taper2Output);
}
} // namespace dhe
