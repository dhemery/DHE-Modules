#include "panels/XycloidPanel.h"

namespace dhe {

XycloidPanel::XycloidPanel(Xycloid *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = widgetRightEdge / 7.F;
  auto column4 = widgetRightEdge - column1;
  auto column2 = (column4 - column1) / 3.F + column1;
  auto column3 = widgetRightEdge - column2;

  auto y = 30.F;
  auto dy = 22.F;

  input(column1, y, Xycloid::WobbleRatioCvInput);
  knob<TinyKnob>(column2, y, Xycloid::WobbleRatioAvKnob);
  knob<LargeKnob>(column3, y, Xycloid::WobbleRatioKnob);
  toggle<2>(column4, y, Xycloid::WobbleRatioModeSwitch);

  y += dy;
  input(column1, y, Xycloid::WobbleDepthCvInput);
  knob<TinyKnob>(column2, y, Xycloid::WobbleDepthAvKnob);
  knob<LargeKnob>(column3, y, Xycloid::WobbleDepthKnob);
  toggle<3>(column4, y, Xycloid::WobbleRatioRangeSwitch);

  y += dy;
  input(column1, y, Xycloid::ThrobSpeedCvInput);
  knob<TinyKnob>(column2, y, Xycloid::ThrobSpeedAvKnob);
  knob<LargeKnob>(column3, y, Xycloid::ThrobSpeedKnob);
  knob<SmallKnob>(column4, y, Xycloid::WobblePhaseKnob);

  y = 82.F;
  dy = 15.F;
  const auto outputPortOffset = 1.25F;

  y += dy;
  input(column1, y, Xycloid::XGainCvInput);
  knob<SmallKnob>(column2, y, Xycloid::XGainKnob);
  toggle<2>(column3, y, Xycloid::XRangeSwitch);
  output(column4, y + outputPortOffset, Xycloid::XOutput);

  y += dy;
  input(column1, y, Xycloid::YGainCvInput);
  knob<SmallKnob>(column2, y, Xycloid::YGainKnob);
  toggle<2>(column3, y, Xycloid::YRangeSwitch);
  output(column4, y + outputPortOffset, Xycloid::YOutput);
}

} // namespace dhe
