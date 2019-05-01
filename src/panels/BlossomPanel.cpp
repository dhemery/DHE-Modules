#include "panels/BlossomPanel.h"

#include "util/gain.h"

namespace DHE {
BlossomPanel::BlossomPanel(Blossom *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = widget_right_edge / 7.f;
  auto column_4 = widget_right_edge - column_1;
  auto column_2 = (column_4 - column_1) / 3.f + column_1;
  auto column_3 = widget_right_edge - column_2;

  auto y = 25.f;
  auto dy = 18.5f;

  input(column_1, y, Blossom::SPIN_CV);
  knob<TinyKnob>(column_2, y, Blossom::SPIN_AV);
  knob<LargeKnob>(column_3, y, Blossom::SPIN_KNOB);

  y += dy;
  input(column_1, y, Blossom::BOUNCE_CV);
  knob<TinyKnob>(column_2, y, Blossom::BOUNCE_AV);
  knob<LargeKnob>(column_3, y, Blossom::BOUNCE_KNOB);
  toggle<2>(column_4, y, Blossom::BOUNCE_LOCK_SWITCH);

  y += dy;
  input(column_1, y, Blossom::DEPTH_CV);
  knob<TinyKnob>(column_2, y, Blossom::DEPTH_AV);
  knob<LargeKnob>(column_3, y, Blossom::DEPTH_KNOB);

  y += dy;
  input(column_1, y, Blossom::PHASE_CV);
  knob<TinyKnob>(column_2, y, Blossom::PHASE_AV);
  knob<LargeKnob>(column_3, y, Blossom::PHASE_KNOB);

  y = 97.f;
  dy = 15.f;
  const auto output_port_offset = 1.25f;

  input(column_1, y, Blossom::X_GAIN_CV);
  knob<SmallKnob>(column_2, y, Blossom::X_GAIN_KNOB);
  toggle<2>(column_3, y, Blossom::X_RANGE_SWITCH);
  output(column_4, y + output_port_offset, Blossom::X_OUT);

  y += dy;
  input(column_1, y, Blossom::Y_GAIN_CV);
  knob<SmallKnob>(column_2, y, Blossom::Y_GAIN_KNOB);
  toggle<2>(column_3, y, Blossom::Y_RANGE_SWITCH);
  output(column_4, y + output_port_offset, Blossom::Y_OUT);
}
} // namespace DHE