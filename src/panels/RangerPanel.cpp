#include "panels/RangerPanel.h"

namespace dhe {

RangerPanel::RangerPanel(Ranger *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = width() / 3.5f + 0.333333333f;
  auto column2 = widgetRightEdge - column1;

  auto y = 24.f;
  auto dy = 16.f;
  auto panelBuffer = 4.f;

  knob<MediumKnob>(column1, y, Ranger::LevelKnob);
  output(column2, y, Ranger::RangerOutput);

  y += dy;
  input(column1, y, Ranger::LevelCvInput);
  knob<TinyKnob>(column2, y, Ranger::LevelAvKnob);

  y += dy + panelBuffer;
  knob<MediumKnob>(column1, y, Ranger::CcwLimitKnob);
  toggle<2>(column2, y, Ranger::CcwLimitRangeSwitch);

  y += dy;
  input(column1, y, Ranger::CcwLimitCvInput);
  knob<TinyKnob>(column2, y, Ranger::CcwLimitAvKnob);

  y += dy + panelBuffer;
  knob<MediumKnob>(column1, y, Ranger::CwLimitKnob);
  toggle<2>(column2, y, Ranger::CwLimitRangeSwitch);

  y += dy;
  input(column1, y, Ranger::CwLimitCvInput);
  knob<TinyKnob>(column2, y, Ranger::CwLimitAvKnob);
}
} // namespace dhe
