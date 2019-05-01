#include "panels/XycloidPanel.h"

#include "util/gain.h"

namespace DHE {

XycloidPanel::XycloidPanel(Xycloid *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = widget_right_edge / 7.f;
  auto column_4 = widget_right_edge - column_1;
  auto column_2 = (column_4 - column_1) / 3.f + column_1;
  auto column_3 = widget_right_edge - column_2;

  auto y = 30.f;
  auto dy = 22.f;

  input(column_1, y, Xycloid::WOBBLE_RATIO_CV);
  knob<TinyKnob>(column_2, y, Xycloid::WOBBLE_RATIO_AV);
  knob<LargeKnob>(column_3, y, Xycloid::WOBBLE_RATIO_KNOB);
  toggle<2>(column_4, y, Xycloid::WOBBLE_RATIO_FREEDOM_SWITCH);

  y += dy;
  input(column_1, y, Xycloid::WOBBLE_DEPTH_CV);
  knob<TinyKnob>(column_2, y, Xycloid::WOBBLE_DEPTH_AV);
  knob<LargeKnob>(column_3, y, Xycloid::WOBBLE_DEPTH_KNOB);
  toggle<3>(column_4, y, Xycloid::WOBBLE_RANGE_SWITCH);

  y += dy;
  input(column_1, y, Xycloid::THROB_SPEED_CV);
  knob<TinyKnob>(column_2, y, Xycloid::THROB_SPEED_AV);
  knob<LargeKnob>(column_3, y, Xycloid::THROB_SPEED_KNOB);
  knob<SmallKnob>(column_4, y, Xycloid::WOBBLE_PHASE_KNOB);

  y = 82.f;
  dy = 15.f;
  const auto output_port_offset = 1.25f;

  y += dy;
  input(column_1, y, Xycloid::X_GAIN_CV);
  knob<SmallKnob>(column_2, y, Xycloid::X_GAIN_KNOB);
  toggle<2>(column_3, y, Xycloid::X_RANGE_SWITCH);
  output(column_4, y + output_port_offset, Xycloid::X_OUT);

  y += dy;
  input(column_1, y, Xycloid::Y_GAIN_CV);
  knob<SmallKnob>(column_2, y, Xycloid::Y_GAIN_KNOB);
  toggle<2>(column_3, y, Xycloid::Y_RANGE_SWITCH);
  output(column_4, y + output_port_offset, Xycloid::Y_OUT);
}

} // namespace DHE
