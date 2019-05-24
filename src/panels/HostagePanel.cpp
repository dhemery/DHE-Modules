#include "panels/HostagePanel.h"

namespace dhe {

HostagePanel::HostagePanel(Hostage *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto column_1 = width() / 4.f + 0.333333f;
  auto column_2 = widget_right_edge / 2.f;
  auto column_3 = widget_right_edge - column_1;

  auto y = 25.f;
  auto dy = 18.5f;

  toggle<2>(column_2, y, Hostage::ModeSwitch);

  y += dy;
  input(column_1, y, Hostage::DurationCvInput);
  toggle<3>(column_3, y, Hostage::DurationRangeSwitch);

  y += dy;
  knob<LargeKnob>(column_2, y, Hostage::DurationKnob);

  y = 82.f;
  dy = 15.f;

  input(column_1, y, Hostage::DeferGateInput);
  output(column_3, y, Hostage::ActiveOutput);

  y += dy;
  input(column_1, y, Hostage::GateInput);
  output(column_3, y, Hostage::EocOutput);

  y += dy;
  input(column_1, y, Hostage::EnvelopeInput);
  output(column_3, y, Hostage::EnvelopeOutput);
}
} // namespace dhe
