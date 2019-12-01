#include "XycloidPanel.h"

#include "Xycloid.h"
#include "widgets/Jacks.h"
#include "widgets/Knobs.h"
#include "widgets/Screws.h"

namespace dhe {

using AvKnob = TinyKnob<XycloidPanel>;
using GainKnob = SmallKnob<XycloidPanel>;
using MainKnob = LargeKnob<XycloidPanel>;

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
  AvKnob::install(this, module, column2, y, Xycloid::SpeedAvKnob);
  MainKnob::install(this, module, column3, y, Xycloid::SpeedKnob);

  y += dy;
  installInput(this, module, column1, y, Xycloid::RatioCvInput);
  AvKnob::install(this, module, column2, y, Xycloid::RatioAvKnob);
  MainKnob::install(this, module, column3, y, Xycloid::RatioKnob);
  toggle<2>(column4, y, Xycloid::FreeRatioSwitch);

  y += dy;
  installInput(this, module, column1, y, Xycloid::DepthCvInput);
  AvKnob::install(this, module, column2, y, Xycloid::DepthAvKnob);
  MainKnob::install(this, module, column3, y, Xycloid::DepthKnob);
  toggle<3>(column4, y, Xycloid::DirectionSwitch);

  y += dy;
  installInput(this, module, column1, y, Xycloid::PhaseCvInput);
  AvKnob::install(this, module, column2, y, Xycloid::PhaseOffsetAvKnob);
  MainKnob::install(this, module, column3, y, Xycloid::PhaseOffsetKnob);

  y = 82.F;
  dy = 15.F;
  auto const outputPortOffset = 1.25F;

  y += dy;
  installInput(this, module, column1, y, Xycloid::XGainCvInput);
  GainKnob::install(this, module, column2, y, Xycloid::XGainKnob);
  toggle<2>(column3, y, Xycloid::XRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Xycloid::XOutput);

  y += dy;
  installInput(this, module, column1, y, Xycloid::YGainCvInput);
  GainKnob::install(this, module, column2, y, Xycloid::YGainKnob);
  toggle<2>(column3, y, Xycloid::YRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Xycloid::YOutput);
}

} // namespace dhe
