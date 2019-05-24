#include "panels/BlossomPanel.h"

namespace dhe {
BlossomPanel::BlossomPanel(Blossom *blossom) : Panel{blossom, hp} {
  auto widget_right_edge = width();

  auto column_1 = widget_right_edge / 7.f;
  auto column_4 = widget_right_edge - column_1;
  auto column_2 = (column_4 - column_1) / 3.f + column_1;
  auto column_3 = widget_right_edge - column_2;

  auto y = 25.f;
  auto dy = 18.5f;

  input(column_1, y, Blossom::SpinCvInput);
  knob<TinyKnob>(column_2, y, Blossom::SpinAvKNob);
  knob<LargeKnob>(column_3, y, Blossom::SpinKnob);

  y += dy;
  input(column_1, y, Blossom::BounceCvInput);
  knob<TinyKnob>(column_2, y, Blossom::BounceAvKnob);
  knob<LargeKnob>(column_3, y, Blossom::BounceKnob);
  toggle<2>(column_4, y, Blossom::BounceFreedomSwitch);

  y += dy;
  input(column_1, y, Blossom::DepthCvInput);
  knob<TinyKnob>(column_2, y, Blossom::DepthAvKnob);
  knob<LargeKnob>(column_3, y, Blossom::DepthKnob);

  y += dy;
  input(column_1, y, Blossom::PhaseCvInput);
  knob<TinyKnob>(column_2, y, Blossom::PhaseAvKnob);
  knob<LargeKnob>(column_3, y, Blossom::PhaseKnob);

  y = 97.f;
  dy = 15.f;
  const auto output_port_offset = 1.25f;

  input(column_1, y, Blossom::XGainCvInput);
  knob<SmallKnob>(column_2, y, Blossom::XGainKnob);
  toggle<2>(column_3, y, Blossom::XRangeSwitch);
  output(column_4, y + output_port_offset, Blossom::XOutput);

  y += dy;
  input(column_1, y, Blossom::YGainCvInput);
  knob<SmallKnob>(column_2, y, Blossom::YGainKnob);
  toggle<2>(column_3, y, Blossom::YRangeSwitch);
  output(column_4, y + output_port_offset, Blossom::YOutput);
}
} // namespace dhe
