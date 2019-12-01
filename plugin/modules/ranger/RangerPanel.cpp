#include "RangerPanel.h"

#include "Ranger.h"
#include "widgets/Jacks.h"
#include "widgets/Knobs.h"
#include "widgets/Screws.h"

namespace dhe {

RangerPanel::RangerPanel(rack::engine::Module *module) {
  setModule(module);
  setPanel(backgroundSvg<RangerPanel>());
  installScrews(this);

  auto constexpr left = hp2mm(1.5F);
  auto const right = hp2mm(hp) - left;

  auto y = 14.5F;
  auto constexpr dy = 15.F;

  y += dy * 0.75F;
  knob<MediumKnob>(left, y, Ranger::LevelKnob);
  installOutput(this, module, right, y, Ranger::RangerOutput);

  y += dy;
  installInput(this, module, left, y, Ranger::LevelCvInput);
  knob<TinyKnob>(right, y, Ranger::LevelAvKnob);

  y += dy / 2.F;
  y += dy * 0.75F;
  knob<MediumKnob>(left, y, Ranger::CcwLimitKnob);
  knob<MediumKnob>(right, y, Ranger::CwLimitKnob);

  y += dy;
  knob<TinyKnob>(left, y, Ranger::CcwLimitAvKnob);
  knob<TinyKnob>(right, y, Ranger::CwLimitAvKnob);

  y += dy;
  installInput(this, module, left, y, Ranger::CcwLimitCvInput);
  installInput(this, module, right, y, Ranger::CwLimitCvInput);

  y += dy;
  toggle<2>(left, y, Ranger::CcwLimitRangeSwitch);
  toggle<2>(right, y, Ranger::CwLimitRangeSwitch);
}
} // namespace dhe
