#include "XycloidPanel.h"

#include "Xycloid.h"
#include "widgets/Knobs.h"
#include "widgets/Screws.h"

namespace dhe {

XycloidPanel::XycloidPanel(rack::engine::Module *module) {
  setModule(module);
  setPanel(backgroundSvg<XycloidPanel>());
  installScrews(this);

  auto constexpr width = hp2mm(hp);

  auto const column1 = width / 7.F;
  auto const column4 = width - column1;
  auto const column2 = (column4 - column1) / 3.F + column1;
  auto const column3 = width - column2;

  auto y = 25.F;
  auto dy = 18.5F;

  installInput(this, module, column1, y, Xycloid::SpeedCvInput);
  install<TinyKnob>(this, module, column2, y, Xycloid::SpeedAvKnob);
  install<LargeKnob>(this, module, column3, y, Xycloid::SpeedKnob);

  y += dy;
  installInput(this, module, column1, y, Xycloid::RatioCvInput);
  install<TinyKnob>(this, module, column2, y, Xycloid::RatioAvKnob);
  install<LargeKnob>(this, module, column3, y, Xycloid::RatioKnob);
  install<Toggle, 2>(this, module, column4, y, Xycloid::FreeRatioSwitch);

  y += dy;
  installInput(this, module, column1, y, Xycloid::DepthCvInput);
  install<TinyKnob>(this, module, column2, y, Xycloid::DepthAvKnob);
  install<LargeKnob>(this, module, column3, y, Xycloid::DepthKnob);
  install<Toggle, 3>(this, module, column4, y, Xycloid::DirectionSwitch);

  y += dy;
  installInput(this, module, column1, y, Xycloid::PhaseCvInput);
  install<TinyKnob>(this, module, column2, y, Xycloid::PhaseOffsetAvKnob);
  install<LargeKnob>(this, module, column3, y, Xycloid::PhaseOffsetKnob);

  y = 82.F;
  dy = 15.F;
  auto const outputPortOffset = 1.25F;

  y += dy;
  installInput(this, module, column1, y, Xycloid::XGainCvInput);
  install<SmallKnob>(this, module, column2, y, Xycloid::XGainKnob);
  install<Toggle, 2>(this, module, column3, y, Xycloid::XRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Xycloid::XOutput);

  y += dy;
  installInput(this, module, column1, y, Xycloid::YGainCvInput);
  install<SmallKnob>(this, module, column2, y, Xycloid::YGainKnob);
  install<Toggle, 2>(this, module, column3, y, Xycloid::YRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Xycloid::YOutput);
}

} // namespace dhe
