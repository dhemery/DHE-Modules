#include "panels/BlossomPanel.h"

namespace dhe {
BlossomPanel::BlossomPanel(Blossom *blossom) : Panel{blossom, hp} {
  auto widgetRightEdge = width();

  auto column1 = widgetRightEdge / 7.F;
  auto column4 = widgetRightEdge - column1;
  auto column2 = (column4 - column1) / 3.F + column1;
  auto column3 = widgetRightEdge - column2;

  auto y = 25.F;
  auto dy = 18.5F;

  input(column1, y, Blossom::SpinCvInput);
  knob<TinyKnob>(column2, y, Blossom::SpinAvKNob);
  knob<LargeKnob>(column3, y, Blossom::SpinKnob);

  y += dy;
  input(column1, y, Blossom::BounceRatioCvInput);
  knob<TinyKnob>(column2, y, Blossom::BounceRatioAvKnob);
  knob<LargeKnob>(column3, y, Blossom::BounceRatioKnob);
  toggle<2>(column4, y, Blossom::BounceRatioModeSwitch);

  y += dy;
  input(column1, y, Blossom::BounceDepthCvInput);
  knob<TinyKnob>(column2, y, Blossom::BounceDepthAvKnob);
  knob<LargeKnob>(column3, y, Blossom::BounceDepthKnob);

  y += dy;
  input(column1, y, Blossom::PhaseCvInput);
  knob<TinyKnob>(column2, y, Blossom::BouncePhaseOffsetAvKnob);
  knob<LargeKnob>(column3, y, Blossom::BouncePhaseOffsetKnob);

  y = 97.F;
  dy = 15.F;
  const auto outputPortOffset = 1.25F;

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
