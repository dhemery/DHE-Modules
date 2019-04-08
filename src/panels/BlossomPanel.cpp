#include <panels/BlossomPanel.h>
#include <util/gain.h>

namespace DHE {
BlossomPanel::BlossomPanel(Blossom *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = widget_right_edge / 7.f;
  auto column_4 = widget_right_edge - column_1;
  auto column_2 = (column_4 - column_1) / 3.f + column_1;
  auto column_3 = widget_right_edge - column_2;

  auto y = 25.f;
  auto dy = 18.5f;

  install(column_1, y, input(Blossom::SPIN_CV));
  install(column_2, y, knob<TinyKnob>(Blossom::SPIN_AV));
  install(column_3, y, knob<LargeKnob>(Blossom::SPIN_KNOB, 0.65f));

  y += dy;
  install(column_1, y, input(Blossom::BOUNCE_CV));
  install(column_2, y, knob<TinyKnob>(Blossom::BOUNCE_AV));
  install(column_3, y, knob<LargeKnob>(Blossom::BOUNCE_KNOB));
  install(column_4, y, toggle<2>(Blossom::BOUNCE_LOCK_SWITCH, 1));

  y += dy;
  install(column_1, y, input(Blossom::DEPTH_CV));
  install(column_2, y, knob<TinyKnob>(Blossom::DEPTH_AV));
  install(column_3, y, knob<LargeKnob>(Blossom::DEPTH_KNOB));

  y += dy;
  install(column_1, y, input(Blossom::PHASE_CV));
  install(column_2, y, knob<TinyKnob>(Blossom::PHASE_AV));
  install(column_3, y, knob<LargeKnob>(Blossom::PHASE_KNOB));

  y = 97.f;
  dy = 15.f;
  const auto output_port_offset = 1.25f;

  auto default_gain = Gain::range.normalize(1.f);

  install(column_1, y, input(Blossom::X_GAIN_CV));
  install(column_2, y, knob<SmallKnob>(Blossom::X_GAIN_KNOB, default_gain));
  install(column_3, y, toggle<2>(Blossom::X_RANGE_SWITCH, 0));
  install(column_4, y + output_port_offset, output(Blossom::X_OUT));

  y += dy;
  install(column_1, y, input(Blossom::Y_GAIN_CV));
  install(column_2, y, knob<SmallKnob>(Blossom::Y_GAIN_KNOB, default_gain));
  install(column_3, y, toggle<2>(Blossom::Y_RANGE_SWITCH, 0));
  install(column_4, y + output_port_offset, output(Blossom::Y_OUT));
}
} // namespace DHE