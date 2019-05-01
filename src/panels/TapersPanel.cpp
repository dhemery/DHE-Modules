#include "panels/TapersPanel.h"

namespace DHE {

TapersPanel::TapersPanel(Tapers *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 5.f + 0.333333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 24.f;
  auto dy = 16.f;
  auto panel_buffer = 4.f;

  install(column_1, y, input(Tapers::LEVEL_1_CV));
  install(column_2, y, knob<TinyKnob>(Tapers::LEVEL_1_AV));
  install(column_3, y, knob<MediumKnob>(Tapers::LEVEL_1_KNOB));
  y += dy;
  install(column_1, y, input(Tapers::CURVE_1_CV));
  install(column_2, y, knob<TinyKnob>(Tapers::CURVE_1_AV));
  install(column_3, y, knob<MediumKnob>(Tapers::CURVE_1_KNOB));
  y += dy;
  install(column_1, y, toggle<2>(Tapers::SHAPE_1_SWITCH, 0));
  install(column_2, y, toggle<2>(Tapers::RANGE_1_SWITCH, 1));
  install(column_3, y, output(Tapers::OUT_1));

  y += dy + panel_buffer;

  install(column_1, y, input(Tapers::LEVEL_2_CV));
  install(column_2, y, knob<TinyKnob>(Tapers::LEVEL_2_AV));
  install(column_3, y, knob<MediumKnob>(Tapers::LEVEL_2_KNOB));
  y += dy;
  install(column_1, y, input(Tapers::CURVE_2_CV));
  install(column_2, y, knob<TinyKnob>(Tapers::CURVE_2_AV));
  install(column_3, y, knob<MediumKnob>(Tapers::CURVE_2_KNOB));
  y += dy;
  install(column_1, y, toggle<2>(Tapers::SHAPE_2_SWITCH, 0));
  install(column_2, y, toggle<2>(Tapers::RANGE_2_SWITCH, 1));
  install(column_3, y, output(Tapers::OUT_2));
}
} // namespace DHE
