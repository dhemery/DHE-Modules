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

  addInput(Jack::input(moduleSlug, module, column1, y, Xycloid::SpeedCvInput));
  addParam(Knob::tiny(moduleSlug, module, column2, y, Xycloid::SpeedAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Xycloid::SpeedKnob));

  y += dy;
  addInput(Jack::input(moduleSlug, module, column1, y, Xycloid::RatioCvInput));
  addParam(Knob::tiny(moduleSlug, module, column2, y, Xycloid::RatioAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Xycloid::RatioKnob));
  addParam(Toggle::stepper(2, moduleSlug, module, column4, y, Xycloid::FreeRatioSwitch));

  y += dy;
  addInput(Jack::input(moduleSlug, module, column1, y, Xycloid::DepthCvInput));
  addParam(Knob::tiny(moduleSlug, module, column2, y, Xycloid::DepthAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Xycloid::DepthKnob));
  addParam(Toggle::stepper(3, moduleSlug, module, column4, y, Xycloid::DirectionSwitch));

  y += dy;
  addInput(Jack::input(moduleSlug, module, column1, y, Xycloid::PhaseCvInput));
  addParam(Knob::tiny(moduleSlug, module, column2, y, Xycloid::PhaseOffsetAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Xycloid::PhaseOffsetKnob));

  y = 82.F;
  dy = 15.F;
  auto const outputPortOffset = 1.25F;

  y += dy;
  addInput(Jack::input(moduleSlug, module, column1, y, Xycloid::XGainCvInput));
  addParam(Knob::small(moduleSlug, module, column2, y, Xycloid::XGainKnob));
  addParam(Toggle::stepper(2, moduleSlug, module, column3, y, Xycloid::XRangeSwitch));
  addOutput(Jack::output(moduleSlug, module, column4, y + outputPortOffset, Xycloid::XOutput));

  y += dy;
  addInput(Jack::input(moduleSlug, module, column1, y, Xycloid::YGainCvInput));
  addParam(Knob::small(moduleSlug, module, column2, y, Xycloid::YGainKnob));
  addParam(Toggle::stepper(2, moduleSlug, module, column3, y, Xycloid::YRangeSwitch));
  addOutput(Jack::output(moduleSlug, module, column4, y + outputPortOffset, Xycloid::YOutput));
}

} // namespace dhe
