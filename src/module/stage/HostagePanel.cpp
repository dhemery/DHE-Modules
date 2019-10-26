#include "module/stage/HostagePanel.h"

namespace dhe {

HostagePanel::HostagePanel(Hostage *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 4.F + 0.333333F;
  auto column2 = widgetRightEdge / 2.F;
  auto column3 = widgetRightEdge - column1;

  auto y = 25.F;
  auto dy = 18.5F;

  toggle<2>(column2, y, Hostage::ModeSwitch);

  y += dy;
  input(column1, y, Hostage::DurationCvInput);
  toggle<3>(column3, y, Hostage::DurationRangeSwitch);

  y += dy;
  knob<LargeKnob>(column2, y, Hostage::DurationKnob);

  y = 82.F;
  dy = 15.F;

  input(column1, y, Hostage::DeferInput);
  output(column3, y, Hostage::ActiveOutput);

  y += dy;
  input(column1, y, Hostage::TriggerInput);
  output(column3, y, Hostage::EocOutput);

  y += dy;
  input(column1, y, Hostage::EnvelopeInput);
  output(column3, y, Hostage::EnvelopeOutput);
}
} // namespace dhe
