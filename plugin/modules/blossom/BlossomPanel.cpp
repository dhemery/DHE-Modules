#include "BlossomPanel.h"

#include "Blossom.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

BlossomPanel::BlossomPanel(rack::engine::Module *module) {
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

  installInput(this, module, column1, y, Blossom::SpeedCvInput);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Blossom::SpeedAvKNob));
  addParam(Knob::large(moduleSlug, module, column3, y, Blossom::SpeedKnob));

  y += dy;
  installInput(this, module, column1, y, Blossom::RatioCvInput);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Blossom::RatioAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Blossom::RatioKnob));
  install<Toggle<2>>(this, module, column4, y, Blossom::FreeRatioSwitch);

  y += dy;
  installInput(this, module, column1, y, Blossom::DepthCvInput);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Blossom::DepthAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Blossom::DepthKnob));

  y += dy;
  installInput(this, module, column1, y, Blossom::PhaseCvInput);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Blossom::PhaseOffsetAvKnob));
  addParam(Knob::large(moduleSlug, module, column3, y, Blossom::PhaseOffsetKnob));

  y = 97.F;
  dy = 15.F;
  auto const outputPortOffset = 1.25F;

  installInput(this, module, column1, y, Blossom::XGainCvInput);
  addParam(Knob::small(moduleSlug, module, column2, y, Blossom::XGainKnob));
  install<Toggle<2>>(this, module, column3, y, Blossom::XRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Blossom::XOutput);

  y += dy;
  installInput(this, module, column1, y, Blossom::YGainCvInput);
  addParam(Knob::small(moduleSlug, module, column2, y, Blossom::YGainKnob));
  install<Toggle<2>>(this, module, column3, y, Blossom::YRangeSwitch);
  installOutput(this, module, column4, y + outputPortOffset, Blossom::YOutput);
}
} // namespace dhe
