#include "panels/XycloidPanel.h"

namespace dhe {

XycloidPanel::XycloidPanel(Xycloid *module) : Panel{module, hp} {
  auto const widgetRightEdge = width();

  auto const column1 = widgetRightEdge / 7.F;
  auto const column4 = widgetRightEdge - column1;
  auto const column2 = (column4 - column1) / 3.F + column1;
  auto const column3 = widgetRightEdge - column2;

  auto y = 25.F;
  auto dy = 18.5F;

  input(column1, y, Xycloid::SpeedCvInput);
  knob<TinyKnob>(column2, y, Xycloid::SpeedAvKnob);
  knob<LargeKnob>(column3, y, Xycloid::SpeedKnob);

  y += dy;
  input(column1, y, Xycloid::RatioCvInput);
  knob<TinyKnob>(column2, y, Xycloid::RatioAvKnob);
  knob<LargeKnob>(column3, y, Xycloid::RatioKnob);
  toggle<2>(column4, y, Xycloid::FreeRatioSwitch);

  y += dy;
  input(column1, y, Xycloid::DepthCvInput);
  knob<TinyKnob>(column2, y, Xycloid::DepthAvKnob);
  knob<LargeKnob>(column3, y, Xycloid::DepthKnob);
  toggle<3>(column4, y, Xycloid::DirectionSwitch);

  y += dy;
  input(column1, y, Xycloid::PhaseCvInput);
  knob<TinyKnob>(column2, y, Xycloid::PhaseOffsetAvKnob);
  knob<LargeKnob>(column3, y, Xycloid::PhaseOffsetKnob);

  y = 82.F;
  dy = 15.F;
  auto const outputPortOffset = 1.25F;

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
