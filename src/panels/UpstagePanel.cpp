#include "panels/UpstagePanel.h"

namespace dhe {

UpstagePanel::UpstagePanel(Upstage *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 4.f + 0.333333333f;
  auto column2 = widgetRightEdge / 2.f;
  auto column3 = widgetRightEdge - column1;

  auto y = 25.f;
  auto dy = 18.5f;

  knob<LargeKnob>(column2, y, Upstage::LevelKnob);

  y += dy;
  input(column1, y, Upstage::LevelCvInput);
  toggle<2>(column3, y, Upstage::LevelRangeSwitch);

  y += dy;
  button(column1, y, Upstage::WaitButton);
  button(column3, y, Upstage::TriggerButton);

  y = 82.f;
  dy = 15.f;

  input(column1, y, Upstage::WaitInput);

  y += dy;
  input(column1, y, Upstage::TriggerInput);
  output(column3, y, Upstage::TriggerOutput);

  y += dy;
  output(column3, y, Upstage::EnvelopeOutput);
}
} // namespace dhe
