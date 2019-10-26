#include "module/ranger/RangerPanel.h"

namespace dhe {

RangerPanel::RangerPanel(Ranger *module) : Panel{module, hp} {
  auto widgetRightEdge = width();

  auto column1 = 1.5F * mmPerHp();
  auto column2 = widgetRightEdge - column1;

  auto y = 14.5F;
  auto dy = 15.F;

  y += dy * 0.75F;
  knob<MediumKnob>(column1, y, Ranger::LevelKnob);
  output(column2, y, Ranger::RangerOutput);

  y += dy;
  input(column1, y, Ranger::LevelCvInput);
  knob<TinyKnob>(column2, y, Ranger::LevelAvKnob);

  y += dy / 2.F;
  y += dy * 0.75F;
  knob<MediumKnob>(column1, y, Ranger::CcwLimitKnob);
  knob<MediumKnob>(column2, y, Ranger::CwLimitKnob);

  y += dy;
  knob<TinyKnob>(column1, y, Ranger::CcwLimitAvKnob);
  knob<TinyKnob>(column2, y, Ranger::CwLimitAvKnob);

  y += dy;
  input(column1, y, Ranger::CcwLimitCvInput);
  input(column2, y, Ranger::CwLimitCvInput);

  y += dy;
  toggle<2>(column1, y, Ranger::CcwLimitRangeSwitch);
  toggle<2>(column2, y, Ranger::CwLimitRangeSwitch);
}
} // namespace dhe
