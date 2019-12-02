#include "TapersPanel.h"

#include "Tapers.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

TapersPanel::TapersPanel(rack::engine::Module *module) {
  auto constexpr hp = 9;

  setModule(module);
  setPanel(backgroundSvg(moduleSlug));
  installScrews(this, hp);

  auto width = hp2mm(hp);

  auto column1 = width / 5.F + 0.333333333F;
  auto column2 = width / 2.F;
  auto column3 = width - column1;

  auto y = 24.F;
  auto dy = 16.F;
  auto panelBuffer = 4.F;

  installInput(this, module, column1, y, Tapers::LevelCvInput1);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Tapers::LevelAvKnob1));
  addParam(Knob::medium(moduleSlug, module, column3, y, Tapers::LevelKnob1));
  y += dy;
  installInput(this, module, column1, y, Tapers::CurveCvInput1);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Tapers::CurveAvKnob1));
  addParam(Knob::medium(moduleSlug, module, column3, y, Tapers::CurveKnob1));
  y += dy;
  addParam(Toggle::stepper(2, moduleSlug, module, column1, y, Tapers::ShapeSwitch1));
  addParam(Toggle::stepper(2, moduleSlug, module, column2, y, Tapers::LevelRangeSwitch1));
  installOutput(this, module, column3, y, Tapers::TaperOutput1);

  y += dy + panelBuffer;

  installInput(this, module, column1, y, Tapers::LevelCvInput2);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Tapers::LevelAvKnob2));
  addParam(Knob::medium(moduleSlug, module, column3, y, Tapers::LevelKnob2));
  y += dy;
  installInput(this, module, column1, y, Tapers::CurveCvInput2);
  addParam(Knob::tiny(moduleSlug, module, column2, y, Tapers::CurveAvKnob2));
  addParam(Knob::medium(moduleSlug, module, column3, y, Tapers::CurveKnob2));
  y += dy;
  addParam(Toggle::stepper(2, moduleSlug, module, column1, y, Tapers::ShapeSwitch2));
  addParam(Toggle::stepper(2, moduleSlug, module, column2, y, Tapers::LevelRangeSwitch2));
  installOutput(this, module, column3, y, Tapers::TaperOutput2);
}
} // namespace dhe
