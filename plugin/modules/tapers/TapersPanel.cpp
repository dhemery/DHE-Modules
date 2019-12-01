#include "TapersPanel.h"

#include "Tapers.h"
#include "widgets/Knobs.h"
#include "widgets/Screws.h"
#include "widgets/Toggles.h"

namespace dhe {

TapersPanel::TapersPanel(rack::engine::Module *module) {
  auto constexpr hp = 9;

  setModule(module);
  setPanel(backgroundSvg<TapersPanel>());
  installScrews(this, hp);

  auto width = hp2mm(hp);

  auto column1 = width / 5.F + 0.333333333F;
  auto column2 = width / 2.F;
  auto column3 = width - column1;

  auto y = 24.F;
  auto dy = 16.F;
  auto panelBuffer = 4.F;

  installInput(this, module, column1, y, Tapers::LevelCvInput1);
  install<TinyKnob>(this, module, column2, y, Tapers::LevelAvKnob1);
  install<MediumKnob>(this, module, column3, y, Tapers::LevelKnob1);
  y += dy;
  installInput(this, module, column1, y, Tapers::CurveCvInput1);
  install<TinyKnob>(this, module, column2, y, Tapers::CurveAvKnob1);
  install<MediumKnob>(this, module, column3, y, Tapers::CurveKnob1);
  y += dy;
  install<Toggle, 2>(this, module, column1, y, Tapers::ShapeSwitch1);
  install<Toggle, 2>(this, module, column2, y, Tapers::LevelRangeSwitch1);
  installOutput(this, module, column3, y, Tapers::TaperOutput1);

  y += dy + panelBuffer;

  installInput(this, module, column1, y, Tapers::LevelCvInput2);
  install<TinyKnob>(this, module, column2, y, Tapers::LevelAvKnob2);
  install<MediumKnob>(this, module, column3, y, Tapers::LevelKnob2);
  y += dy;
  installInput(this, module, column1, y, Tapers::CurveCvInput2);
  install<TinyKnob>(this, module, column2, y, Tapers::CurveAvKnob2);
  install<MediumKnob>(this, module, column3, y, Tapers::CurveKnob2);
  y += dy;
  install<Toggle, 2>(this, module, column1, y, Tapers::ShapeSwitch2);
  install<Toggle, 2>(this, module, column2, y, Tapers::LevelRangeSwitch2);
  installOutput(this, module, column3, y, Tapers::TaperOutput2);
}
} // namespace dhe
