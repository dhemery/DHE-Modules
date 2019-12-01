#include "BlossomPanel.h"

#include "widgets/Jacks.h"
#include "widgets/Knobs.h"
#include "widgets/Screws.h"

namespace dhe {

using AvKnob = TinyKnob<BlossomPanel>;
using GainKnob = SmallKnob<BlossomPanel>;
using MainKnob = LargeKnob<BlossomPanel>;

BlossomPanel::BlossomPanel(rack::engine::Module *module) {
  setModule(module);
  setPanel(backgroundSvg<BlossomPanel>());
  installScrews(this);

  auto const width = hp2mm(hp);

  auto const column1 = width / 7.F;
  auto const column4 = width - column1;
  auto const column2 = (column4 - column1) / 3.F + column1;
  auto const column3 = width - column2;

  auto y = 25.F;
  auto dy = 18.5F;

  installInput(this, module, column1, y, Blossom::SpeedCvInput);
  AvKnob::install(this, module, column2, y, Blossom::SpeedAvKNob);
  MainKnob::install(this, module, column3, y, Blossom::SpeedKnob);

  y += dy;
  installInput(this, module, column1, y, Blossom::RatioCvInput);
  AvKnob::install(this, module, column2, y, Blossom::RatioAvKnob);
  MainKnob::install(this, module, column3, y, Blossom::RatioKnob);
  toggle<2>(column4, y, Blossom::FreeRatioSwitch);

  y += dy;
  installInput(this, module, column1, y, Blossom::DepthCvInput);
  AvKnob::install(this, module, column2, y, Blossom::DepthAvKnob);
  MainKnob::install(this, module, column3, y, Blossom::DepthKnob);

  y += dy;
  installInput(this, module, column1, y, Blossom::PhaseCvInput);
  AvKnob::install(this, module, column2, y, Blossom::PhaseOffsetAvKnob);
  MainKnob::install(this, module, column3, y, Blossom::PhaseOffsetKnob);

  y = 97.F;
  dy = 15.F;
  auto const outputPortOffset = 1.25F;

  installInput(this, module, column1, y, Blossom::XGainCvInput);
  GainKnob::install(this, module, column2, y, Blossom::XGainKnob);
  toggle<2>(column3, y, Blossom::XRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Blossom::XOutput);

  y += dy;
  installInput(this, module, column1, y, Blossom::YGainCvInput);
  GainKnob::install(this, module, column2, y, Blossom::YGainKnob);
  toggle<2>(column3, y, Blossom::YRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Blossom::YOutput);
}
} // namespace dhe
