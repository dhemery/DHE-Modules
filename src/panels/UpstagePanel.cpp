#include "panels/UpstagePanel.h"

namespace dhe {

UpstagePanel::UpstagePanel(Upstage *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 4.F + 0.333333333F;
  auto column2 = widgetRightEdge / 2.F;
  auto column3 = widgetRightEdge - column1;

  auto y = 25.F;
  auto dy = 18.5F;

  knob<LargeKnob>(column2, y, Upstage::LevelKnob);

  y += dy;
  input(column1, y, Upstage::LevelCvInput);
  toggle<2>(column3, y, Upstage::LevelRangeSwitch);

  y += dy;
  button(column1, y, Upstage::WaitButton);
  button(column3, y, Upstage::TriggerButton);

  y = 82.F;
  dy = 15.F;

  input(column1, y, Upstage::WaitInput);

  y += dy;
  input(column1, y, Upstage::TriggerInput);
  output(column3, y, Upstage::TriggerOutput);

  y += dy;
  output(column3, y, Upstage::EnvelopeOutput);
}
} // namespace dhe
