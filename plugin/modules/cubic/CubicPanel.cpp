#include "CubicPanel.h"

#include "Cubic.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

CubicPanel::CubicPanel(rack::engine::Module *module) {
  auto constexpr hp = 5;

  setModule(module);
  setPanel(backgroundSvg(moduleSlug));
  installScrews(this, hp);

  auto width = hp2mm(hp);

  auto column1 = width / 4.F + 0.333333F;
  auto column2 = width - column1;

  auto y = 20.F;
  auto dy = 15.F;

  installInput(this, module, column1, y, Cubic::ACoefficientCvInput);
  addParam(Knob::small(moduleSlug, module, column2, y, Cubic::ACoefficientKnob));

  y += dy;
  installInput(this, module, column1, y, Cubic::BCoefficientCvInput);
  addParam(Knob::small(moduleSlug, module, column2, y, Cubic::BCoefficientKnob));

  y += dy;
  installInput(this, module, column1, y, Cubic::CCoefficientCvInput);
  addParam(Knob::small(moduleSlug, module, column2, y, Cubic::CCoefficientKnob));

  y += dy;
  installInput(this, module, column1, y, Cubic::DCoefficientCvInput);
  addParam(Knob::small(moduleSlug, module, column2, y, Cubic::DCoefficientKnob));

  y = 82.F;
  addParam(Knob::small(moduleSlug, module, column1, y, Cubic::InputGainKnob));
  addParam(Knob::small(moduleSlug, module, column2, y, Cubic::OutputGainKnob));

  y += dy;
  installInput(this, module, column1, y, Cubic::InputGainCvInput);
  installInput(this, module, column2, y, Cubic::OutputGainCvInput);

  y += dy;
  installInput(this, module, column1, y, Cubic::CubicInput);
  installOutput(this, module, column2, y, Cubic::CubicOutput);
}
} // namespace dhe
