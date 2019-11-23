#include "RangerPanel.h"

namespace dhe {

RangerPanel::RangerPanel(Ranger *module) : Panel{module, hp} {
  auto constexpr left = hp2mm(1.5F);
  auto const right = width() - left;

  auto y = 14.5F;
  auto constexpr dy = 15.F;

  y += dy * 0.75F;
  knob<MediumKnob>(left, y, Ranger::LevelKnob);
  output(right, y, Ranger::RangerOutput);

  y += dy;
  input(left, y, Ranger::LevelCvInput);
  knob<TinyKnob>(right, y, Ranger::LevelAvKnob);

  y += dy / 2.F;
  y += dy * 0.75F;
  knob<MediumKnob>(left, y, Ranger::CcwLimitKnob);
  knob<MediumKnob>(right, y, Ranger::CwLimitKnob);

  y += dy;
  knob<TinyKnob>(left, y, Ranger::CcwLimitAvKnob);
  knob<TinyKnob>(right, y, Ranger::CwLimitAvKnob);

  y += dy;
  input(left, y, Ranger::CcwLimitCvInput);
  input(right, y, Ranger::CwLimitCvInput);

  y += dy;
  toggle<2>(left, y, Ranger::CcwLimitRangeSwitch);
  toggle<2>(right, y, Ranger::CwLimitRangeSwitch);
}
} // namespace dhe
