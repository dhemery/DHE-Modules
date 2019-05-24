#include "panels/HostagePanel.h"

namespace dhe {

HostagePanel::HostagePanel(Hostage *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 4.f + 0.333333f;
  auto column2 = widgetRightEdge / 2.f;
  auto column3 = widgetRightEdge - column1;

  auto y = 25.f;
  auto dy = 18.5f;

  toggle<2>(column2, y, Hostage::ModeSwitch);

  y += dy;
  input(column1, y, Hostage::DurationCvInput);
  toggle<3>(column3, y, Hostage::DurationRangeSwitch);

  y += dy;
  knob<LargeKnob>(column2, y, Hostage::DurationKnob);

  y = 82.f;
  dy = 15.f;

  input(column1, y, Hostage::DeferGateInput);
  output(column3, y, Hostage::ActiveOutput);

  y += dy;
  input(column1, y, Hostage::GateInput);
  output(column3, y, Hostage::EocOutput);

  y += dy;
  input(column1, y, Hostage::EnvelopeInput);
  output(column3, y, Hostage::EnvelopeOutput);
}
} // namespace dhe
