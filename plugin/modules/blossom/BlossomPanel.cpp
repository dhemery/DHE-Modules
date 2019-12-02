#include "BlossomPanel.h"

#include "Blossom.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

BlossomPanel::BlossomPanel(rack::engine::Module *module) {
  auto constexpr slug = "blossom";
  auto constexpr hp = 11;

  setModule(module);
  setPanel(backgroundSvg(slug));
  installScrews(this, hp);

  auto constexpr width = hp2mm(hp);

  auto constexpr column1 = width / 7.F;
  auto constexpr column4 = width - column1;
  auto constexpr column2 = (column4 - column1) / 3.F + column1;
  auto constexpr column3 = width - column2;

  auto y = 25.F;
  auto dy = 18.5F;

  addInput(Jack::input(slug, module, column1, y, Blossom::SpeedCvInput));
  addParam(Knob::tiny(slug, module, column2, y, Blossom::SpeedAvKNob));
  addParam(Knob::large(slug, module, column3, y, Blossom::SpeedKnob));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Blossom::RatioCvInput));
  addParam(Knob::tiny(slug, module, column2, y, Blossom::RatioAvKnob));
  addParam(Knob::large(slug, module, column3, y, Blossom::RatioKnob));
  addParam(Toggle::stepper(2, slug, module, column4, y, Blossom::FreeRatioSwitch));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Blossom::DepthCvInput));
  addParam(Knob::tiny(slug, module, column2, y, Blossom::DepthAvKnob));
  addParam(Knob::large(slug, module, column3, y, Blossom::DepthKnob));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Blossom::PhaseCvInput));
  addParam(Knob::tiny(slug, module, column2, y, Blossom::PhaseOffsetAvKnob));
  addParam(Knob::large(slug, module, column3, y, Blossom::PhaseOffsetKnob));

  y = 97.F;
  dy = 15.F;
  auto constexpr outputPortOffset = 1.25F;

  addInput(Jack::input(slug, module, column1, y, Blossom::XGainCvInput));
  addParam(Knob::small(slug, module, column2, y, Blossom::XGainKnob));
  addParam(Toggle::stepper(2, slug, module, column3, y, Blossom::XRangeSwitch));
  addOutput(Jack::output(slug, module, column4, y + outputPortOffset, Blossom::XOutput));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Blossom::YGainCvInput));
  addParam(Knob::small(slug, module, column2, y, Blossom::YGainKnob));
  addParam(Toggle::stepper(2, slug, module, column3, y, Blossom::YRangeSwitch));
  addOutput(Jack::output(slug, module, column4, y + outputPortOffset, Blossom::YOutput));
}
} // namespace dhe
