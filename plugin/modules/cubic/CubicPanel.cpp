#include "CubicPanel.h"

#include "Cubic.h"
#include "widgets/Knobs.h"
#include "widgets/Screws.h"

namespace dhe {

CubicPanel::CubicPanel(rack::engine::Module *module) {
  setModule(module);
  setPanel(backgroundSvg<CubicPanel>());
  installScrews(this);

  auto width = hp2mm(hp);

  auto column1 = width / 4.F + 0.333333F;
  auto column2 = width - column1;

  auto y = 20.F;
  auto dy = 15.F;

  installInput(this, module, column1, y, Cubic::ACoefficientCvInput);
  install<SmallKnob>(this, module, column2, y, Cubic::ACoefficientKnob);

  y += dy;
  installInput(this, module, column1, y, Cubic::BCoefficientCvInput);
  install<SmallKnob>(this, module, column2, y, Cubic::BCoefficientKnob);

  y += dy;
  installInput(this, module, column1, y, Cubic::CCoefficientCvInput);
  install<SmallKnob>(this, module, column2, y, Cubic::CCoefficientKnob);

  y += dy;
  installInput(this, module, column1, y, Cubic::DCoefficientCvInput);
  install<SmallKnob>(this, module, column2, y, Cubic::DCoefficientKnob);

  y = 82.F;
  install<SmallKnob>(this, module, column1, y, Cubic::InputGainKnob);
  install<SmallKnob>(this, module, column2, y, Cubic::OutputGainKnob);

  y += dy;
  installInput(this, module, column1, y, Cubic::InputGainCvInput);
  installInput(this, module, column2, y, Cubic::OutputGainCvInput);

  y += dy;
  installInput(this, module, column1, y, Cubic::CubicInput);
  installOutput(this, module, column2, y, Cubic::CubicOutput);
}
} // namespace dhe
