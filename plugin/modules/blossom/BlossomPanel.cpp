#include "BlossomPanel.h"

namespace dhe {
BlossomPanel::BlossomPanel(Blossom *blossom) : Panel{blossom, hp} {
  auto const widgetRightEdge = width();

  auto const column1 = widgetRightEdge / 7.F;
  auto const column4 = widgetRightEdge - column1;
  auto const column2 = (column4 - column1) / 3.F + column1;
  auto const column3 = widgetRightEdge - column2;

  auto y = 25.F;
  auto dy = 18.5F;

  input(column1, y, Blossom::SpeedCvInput);
  knob<TinyKnob>(column2, y, Blossom::SpeedAvKNob);
  knob<LargeKnob>(column3, y, Blossom::SpeedKnob);

  y += dy;
  input(column1, y, Blossom::RatioCvInput);
  knob<TinyKnob>(column2, y, Blossom::RatioAvKnob);
  knob<LargeKnob>(column3, y, Blossom::RatioKnob);
  toggle<2>(column4, y, Blossom::FreeRatioSwitch);

  y += dy;
  input(column1, y, Blossom::DepthCvInput);
  knob<TinyKnob>(column2, y, Blossom::DepthAvKnob);
  knob<LargeKnob>(column3, y, Blossom::DepthKnob);

  y += dy;
  input(column1, y, Blossom::PhaseCvInput);
  knob<TinyKnob>(column2, y, Blossom::PhaseOffsetAvKnob);
  knob<LargeKnob>(column3, y, Blossom::PhaseOffsetKnob);

  y = 97.F;
  dy = 15.F;
  auto const outputPortOffset = 1.25F;

  input(column1, y, Blossom::XGainCvInput);
  knob<SmallKnob>(column2, y, Blossom::XGainKnob);
  toggle<2>(column3, y, Blossom::XRangeSwitch);
  output(column4, y + outputPortOffset, Blossom::XOutput);

  y += dy;
  input(column1, y, Blossom::YGainCvInput);
  knob<SmallKnob>(column2, y, Blossom::YGainKnob);
  toggle<2>(column3, y, Blossom::YRangeSwitch);
  output(column4, y + outputPortOffset, Blossom::YOutput);
}
} // namespace dhe
