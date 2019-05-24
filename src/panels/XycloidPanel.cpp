#include "panels/XycloidPanel.h"

namespace dhe {

XycloidPanel::XycloidPanel(Xycloid *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = widget_right_edge / 7.f;
  auto column_4 = widget_right_edge - column_1;
  auto column_2 = (column_4 - column_1) / 3.f + column_1;
  auto column_3 = widget_right_edge - column_2;

  auto y = 30.f;
  auto dy = 22.f;

  input(column_1, y, Xycloid::WobbleRatioCvInput);
  knob<TinyKnob>(column_2, y, Xycloid::WobbleRatioAvKnob);
  knob<LargeKnob>(column_3, y, Xycloid::WobbleRatioKnob);
  toggle<2>(column_4, y, Xycloid::WobbleFreedomSwitch);

  y += dy;
  input(column_1, y, Xycloid::WobbleDepthCvInput);
  knob<TinyKnob>(column_2, y, Xycloid::WobbleDepthAvKnob);
  knob<LargeKnob>(column_3, y, Xycloid::WobbleDepthKnob);
  toggle<3>(column_4, y, Xycloid::WobbleRatioRangeSwitch);

  y += dy;
  input(column_1, y, Xycloid::ThrobSpeedCvInput);
  knob<TinyKnob>(column_2, y, Xycloid::ThrobSpeedAvKnob);
  knob<LargeKnob>(column_3, y, Xycloid::ThrobSpeedKnob);
  knob<SmallKnob>(column_4, y, Xycloid::WobblePhaseKnob);

  y = 82.f;
  dy = 15.f;
  const auto output_port_offset = 1.25f;

  y += dy;
  input(column_1, y, Xycloid::XGainCvInput);
  knob<SmallKnob>(column_2, y, Xycloid::XGainKnob);
  toggle<2>(column_3, y, Xycloid::XRangeSwitch);
  output(column_4, y + output_port_offset, Xycloid::XOutput);

  y += dy;
  input(column_1, y, Xycloid::YGainCvInput);
  knob<SmallKnob>(column_2, y, Xycloid::YGainKnob);
  toggle<2>(column_3, y, Xycloid::YRangeSwitch);
  output(column_4, y + output_port_offset, Xycloid::YOutput);
}

} // namespace dhe
