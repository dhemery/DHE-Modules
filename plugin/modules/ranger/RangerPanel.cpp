#include "RangerPanel.h"

#include "Ranger.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

RangerPanel::RangerPanel(rack::engine::Module *module) {
  auto const slug = std::string{"ranger"};
  auto constexpr hp = 6;

  setModule(module);
  setPanel(backgroundSvg(slug));
  installScrews(this, hp);

  auto constexpr left = hp2mm(1.5F);
  auto const right = hp2mm(hp) - left;

  auto y = 14.5F;
  auto constexpr dy = 15.F;

  y += dy * 0.75F;
  addParam(Knob::medium(slug, module, left, y, Ranger::LevelKnob));
  addOutput(Jack::output(slug, module, right, y, Ranger::RangerOutput));

  y += dy;
  addInput(Jack::input(slug, module, left, y, Ranger::LevelCvInput));
  addParam(Knob::tiny(slug, module, right, y, Ranger::LevelAvKnob));

  y += dy / 2.F;
  y += dy * 0.75F;
  addParam(Knob::medium(slug, module, left, y, Ranger::CcwLimitKnob));
  addParam(Knob::medium(slug, module, right, y, Ranger::CwLimitKnob));

  y += dy;
  addParam(Knob::tiny(slug, module, left, y, Ranger::CcwLimitAvKnob));
  addParam(Knob::tiny(slug, module, right, y, Ranger::CwLimitAvKnob));

  y += dy;
  addInput(Jack::input(slug, module, left, y, Ranger::CcwLimitCvInput));
  addInput(Jack::input(slug, module, right, y, Ranger::CwLimitCvInput));

  y += dy;
  addParam(Toggle::stepper(2, slug, module, left, y, Ranger::CcwLimitRangeSwitch));
  addParam(Toggle::stepper(2, slug, module, right, y, Ranger::CwLimitRangeSwitch));
}
} // namespace dhe
