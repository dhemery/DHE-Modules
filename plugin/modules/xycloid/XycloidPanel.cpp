#include "XycloidPanel.h"

#include "Xycloid.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

XycloidPanel::XycloidPanel(rack::engine::Module *module) {
  auto constexpr hp = 11;

  setModule(module);
  setPanel(backgroundSvg(moduleSlug));
  installScrews(this, hp);

  auto constexpr width = hp2mm(hp);

  auto constexpr column1 = width / 7.F;
  auto constexpr column4 = width - column1;
  auto constexpr column2 = (column4 - column1) / 3.F + column1;
  auto constexpr column3 = width - column2;

  auto y = 25.F;
  auto dy = 18.5F;

  installInput(this, module, column1, y, Xycloid::SpeedCvInput);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Xycloid::SpeedAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Xycloid::SpeedKnob));

  y += dy;
  installInput(this, module, column1, y, Xycloid::RatioCvInput);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Xycloid::RatioAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Xycloid::RatioKnob));
  install<Toggle<2>>(this, module, column4, y, Xycloid::FreeRatioSwitch);

  y += dy;
  installInput(this, module, column1, y, Xycloid::DepthCvInput);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Xycloid::DepthAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Xycloid::DepthKnob));
  install<Toggle<3>>(this, module, column4, y, Xycloid::DirectionSwitch);

  y += dy;
  installInput(this, module, column1, y, Xycloid::PhaseCvInput);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Xycloid::PhaseOffsetAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Xycloid::PhaseOffsetKnob));

  y = 82.F;
  dy = 15.F;
  auto const outputPortOffset = 1.25F;

  y += dy;
  installInput(this, module, column1, y, Xycloid::XGainCvInput);
  addParam(Knob::small(moduleSlug, module, column2, y, Xycloid::XGainKnob));
  install<Toggle<2>>(this, module, column3, y, Xycloid::XRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Xycloid::XOutput);

  y += dy;
  installInput(this, module, column1, y, Xycloid::YGainCvInput);
  addParam(Knob::small(moduleSlug, module, column2, y, Xycloid::YGainKnob));
  install<Toggle<2>>(this, module, column3, y, Xycloid::YRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Xycloid::YOutput);
}

} // namespace dhe
