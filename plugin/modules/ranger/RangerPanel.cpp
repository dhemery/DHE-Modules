#include "RangerPanel.h"

#include "Ranger.h"
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
  install<LargeKnob>(this, module, left, y, Ranger::LevelKnob);
  installOutput(this, module, right, y, Ranger::RangerOutput);

  y += dy;
  installInput(this, module, left, y, Ranger::LevelCvInput);
  install<TinyKnob>(this, module, right, y, Ranger::LevelAvKnob);

  y += dy / 2.F;
  y += dy * 0.75F;
  install<LargeKnob>(this, module, left, y, Ranger::CcwLimitKnob);
  install<LargeKnob>(this, module, right, y, Ranger::CwLimitKnob);

  y += dy;
  install<TinyKnob>(this, module, left, y, Ranger::CcwLimitAvKnob);
  install<TinyKnob>(this, module, right, y, Ranger::CwLimitAvKnob);

  y += dy;
  installInput(this, module, left, y, Ranger::CcwLimitCvInput);
  installInput(this, module, right, y, Ranger::CwLimitCvInput);

  y += dy;
  install<Toggle, 2>(this, module, left, y, Ranger::CcwLimitRangeSwitch);
  install<Toggle, 2>(this, module, right, y, Ranger::CwLimitRangeSwitch);
}
} // namespace dhe
