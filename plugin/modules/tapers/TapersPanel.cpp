#include "TapersPanel.h"

#include "Tapers.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

TapersPanel::TapersPanel(rack::engine::Module *module) {
  auto constexpr slug = "tapers";
  auto constexpr hp = 9;

  setModule(module);
  setPanel(backgroundSvg(slug));
  installScrews(this, hp);

  auto constexpr width = hp2mm(hp);

  auto constexpr column1 = width / 5.F + 0.333333333F;
  auto constexpr column2 = width / 2.F;
  auto constexpr column3 = width - column1;

  auto constexpr dy = 16.F;
  auto constexpr panelBuffer = 4.F;
  auto y = 24.F;

  addInput(Jack::input(slug, module, column1, y, Tapers::LevelCvInput1));
  addParam(Knob::tiny(slug, module, column2, y, Tapers::LevelAvKnob1));
  addParam(Knob::medium(slug, module, column3, y, Tapers::LevelKnob1));
  y += dy;
  addInput(Jack::input(slug, module, column1, y, Tapers::CurveCvInput1));
  addParam(Knob::tiny(slug, module, column2, y, Tapers::CurveAvKnob1));
  addParam(Knob::medium(slug, module, column3, y, Tapers::CurveKnob1));
  y += dy;
  addParam(Toggle::stepper(2, slug, module, column1, y, Tapers::ShapeSwitch1));
  addParam(Toggle::stepper(2, slug, module, column2, y, Tapers::LevelRangeSwitch1));
  addOutput(Jack::output(slug, module, column3, y, Tapers::TaperOutput1));

  y += dy + panelBuffer;

  addInput(Jack::input(slug, module, column1, y, Tapers::LevelCvInput2));
  addParam(Knob::tiny(slug, module, column2, y, Tapers::LevelAvKnob2));
  addParam(Knob::medium(slug, module, column3, y, Tapers::LevelKnob2));
  y += dy;
  addInput(Jack::input(slug, module, column1, y, Tapers::CurveCvInput2));
  addParam(Knob::tiny(slug, module, column2, y, Tapers::CurveAvKnob2));
  addParam(Knob::medium(slug, module, column3, y, Tapers::CurveKnob2));
  y += dy;
  addParam(Toggle::stepper(2, slug, module, column1, y, Tapers::ShapeSwitch2));
  addParam(Toggle::stepper(2, slug, module, column2, y, Tapers::LevelRangeSwitch2));
  addOutput(Jack::output(slug, module, column3, y, Tapers::TaperOutput2));
}
} // namespace dhe
