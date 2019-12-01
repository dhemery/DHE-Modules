#include "TapersPanel.h"

#include "Tapers.h"

namespace dhe {

TapersPanel::TapersPanel(rack::engine::Module *module) {
  setModule(module);
  setPanel(backgroundSvg<TapersPanel>());
  installScrews(this);

  auto width = hp2mm(hp);

  auto column1 = width / 5.F + 0.333333333F;
  auto column2 = width / 2.F;
  auto column3 = width - column1;

  auto y = 24.F;
  auto dy = 16.F;
  auto panelBuffer = 4.F;

  input(column1, y, Tapers::LevelCvInput1);
  knob<TinyKnob>(column2, y, Tapers::LevelAvKnob1);
  knob<MediumKnob>(column3, y, Tapers::LevelKnob1);
  y += dy;
  input(column1, y, Tapers::CurveCvInput1);
  knob<TinyKnob>(column2, y, Tapers::CurveAvKnob1);
  knob<MediumKnob>(column3, y, Tapers::CurveKnob1);
  y += dy;
  toggle<2>(column1, y, Tapers::ShapeSwitch1);
  toggle<2>(column2, y, Tapers::LevelRangeSwitch1);
  output(column3, y, Tapers::TaperOutput1);

  y += dy + panelBuffer;

  input(column1, y, Tapers::LevelCvInput2);
  knob<TinyKnob>(column2, y, Tapers::LevelAvKnob2);
  knob<MediumKnob>(column3, y, Tapers::LevelKnob2);
  y += dy;
  input(column1, y, Tapers::CurveCvInput2);
  knob<TinyKnob>(column2, y, Tapers::CurveAvKnob2);
  knob<MediumKnob>(column3, y, Tapers::CurveKnob2);
  y += dy;
  toggle<2>(column1, y, Tapers::ShapeSwitch2);
  toggle<2>(column2, y, Tapers::LevelRangeSwitch2);
  output(column3, y, Tapers::TaperOutput2);
}
} // namespace dhe
