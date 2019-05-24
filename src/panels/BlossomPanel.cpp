#include "panels/BlossomPanel.h"

namespace dhe {
BlossomPanel::BlossomPanel(Blossom *blossom) : Panel{blossom, hp} {
  auto widgetRightEdge = width();

  auto column1 = widgetRightEdge / 7.f;
  auto column4 = widgetRightEdge - column1;
  auto column2 = (column4 - column1) / 3.f + column1;
  auto column3 = widgetRightEdge - column2;

  auto y = 25.f;
  auto dy = 18.5f;

  input(column1, y, Blossom::SpinCvInput);
  knob<TinyKnob>(column2, y, Blossom::SpinAvKNob);
  knob<LargeKnob>(column3, y, Blossom::SpinKnob);

  y += dy;
  input(column1, y, Blossom::BounceCvInput);
  knob<TinyKnob>(column2, y, Blossom::BounceAvKnob);
  knob<LargeKnob>(column3, y, Blossom::BounceKnob);
  toggle<2>(column4, y, Blossom::BounceFreedomSwitch);

  y += dy;
  input(column1, y, Blossom::DepthCvInput);
  knob<TinyKnob>(column2, y, Blossom::DepthAvKnob);
  knob<LargeKnob>(column3, y, Blossom::DepthKnob);

  y += dy;
  input(column1, y, Blossom::PhaseCvInput);
  knob<TinyKnob>(column2, y, Blossom::PhaseAvKnob);
  knob<LargeKnob>(column3, y, Blossom::PhaseKnob);

  y = 97.f;
  dy = 15.f;
  const auto outputPortOffset = 1.25f;

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
