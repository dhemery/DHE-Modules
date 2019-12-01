#include "RangerPanel.h"

#include "Ranger.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

RangerPanel::RangerPanel(rack::engine::Module *module) {
  auto constexpr hp = 6;

  setModule(module);
  setPanel(backgroundSvg(moduleSlug));
  installScrews(this, hp);

  auto constexpr left = hp2mm(1.5F);
  auto const right = hp2mm(hp) - left;

  auto y = 14.5F;
  auto constexpr dy = 15.F;

  y += dy * 0.75F;
  addParam(Knob::medium(moduleSlug, module, left, y, Ranger::LevelKnob));
  installOutput(this, module, right, y, Ranger::RangerOutput);

  y += dy;
  installInput(this, module, left, y, Ranger::LevelCvInput);
  addParam(Knob::tiny(moduleSlug, module, right, y, Ranger::LevelAvKnob));

  y += dy / 2.F;
  y += dy * 0.75F;
  addParam(Knob::medium(moduleSlug, module, left, y, Ranger::CcwLimitKnob));
  addParam(Knob::medium(moduleSlug, module, right, y, Ranger::CwLimitKnob));

  y += dy;
  addParam(Knob::tiny(moduleSlug, module, left, y, Ranger::CcwLimitAvKnob));
  addParam(Knob::tiny(moduleSlug, module, right, y, Ranger::CwLimitAvKnob));

  y += dy;
  installInput(this, module, left, y, Ranger::CcwLimitCvInput);
  installInput(this, module, right, y, Ranger::CwLimitCvInput);

  y += dy;
  install<Toggle<2>>(this, module, left, y, Ranger::CcwLimitRangeSwitch);
  install<Toggle<2>>(this, module, right, y, Ranger::CwLimitRangeSwitch);
}
} // namespace dhe
