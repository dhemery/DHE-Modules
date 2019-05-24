#include "panels/TapersPanel.h"

namespace dhe {

TapersPanel::TapersPanel(Tapers *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 5.f + 0.333333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 24.f;
  auto dy = 16.f;
  auto panel_buffer = 4.f;

  input(column_1, y, Tapers::Level1Cv);
  knob<TinyKnob>(column_2, y, Tapers::LevelAvKnob1);
  knob<MediumKnob>(column_3, y, Tapers::LevelKnob1);
  y += dy;
  input(column_1, y, Tapers::Curve1Cv);
  knob<TinyKnob>(column_2, y, Tapers::CurveAvKnob1);
  knob<MediumKnob>(column_3, y, Tapers::CurveKnob1);
  y += dy;
  toggle<2>(column_1, y, Tapers::ShapeSwitch1);
  toggle<2>(column_2, y, Tapers::LevelRangeSwitch1);
  output(column_3, y, Tapers::Taper1Output);

  y += dy + panel_buffer;

  input(column_1, y, Tapers::Level2Cv);
  knob<TinyKnob>(column_2, y, Tapers::LevelAvKnob2);
  knob<MediumKnob>(column_3, y, Tapers::LevelKnob2);
  y += dy;
  input(column_1, y, Tapers::Curve2Cv);
  knob<TinyKnob>(column_2, y, Tapers::Curve2Av);
  knob<MediumKnob>(column_3, y, Tapers::Curve2Knob);
  y += dy;
  toggle<2>(column_1, y, Tapers::Shape2Switch);
  toggle<2>(column_2, y, Tapers::LevelRangeSwitch2);
  output(column_3, y, Tapers::Taper2Output);
}
} // namespace dhe
