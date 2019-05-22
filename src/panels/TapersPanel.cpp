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

  input(column_1, y, Tapers::LEVEL_1_CV);
  knob<TinyKnob>(column_2, y, Tapers::LEVEL_1_AV);
  knob<MediumKnob>(column_3, y, Tapers::LEVEL_1_KNOB);
  y += dy;
  input(column_1, y, Tapers::CURVE_1_CV);
  knob<TinyKnob>(column_2, y, Tapers::CURVE_1_AV);
  knob<MediumKnob>(column_3, y, Tapers::CURVE_1_KNOB);
  y += dy;
  toggle<2>(column_1, y, Tapers::SHAPE_1_SWITCH);
  toggle<2>(column_2, y, Tapers::RANGE_1_SWITCH);
  output(column_3, y, Tapers::OUT_1);

  y += dy + panel_buffer;

  input(column_1, y, Tapers::LEVEL_2_CV);
  knob<TinyKnob>(column_2, y, Tapers::LEVEL_2_AV);
  knob<MediumKnob>(column_3, y, Tapers::LEVEL_2_KNOB);
  y += dy;
  input(column_1, y, Tapers::CURVE_2_CV);
  knob<TinyKnob>(column_2, y, Tapers::CURVE_2_AV);
  knob<MediumKnob>(column_3, y, Tapers::CURVE_2_KNOB);
  y += dy;
  toggle<2>(column_1, y, Tapers::SHAPE_2_SWITCH);
  toggle<2>(column_2, y, Tapers::RANGE_2_SWITCH);
  output(column_3, y, Tapers::OUT_2);
}
} // namespace dhe
