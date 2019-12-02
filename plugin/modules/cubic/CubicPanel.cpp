#include "CubicPanel.h"

#include "Cubic.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

CubicPanel::CubicPanel(rack::engine::Module *module) {
  auto const slug = std::string{"cubic"};
  auto constexpr hp = 5;

  setModule(module);
  setPanel(backgroundSvg(slug));
  installScrews(this, hp);

  auto width = hp2mm(hp);

  auto column1 = width / 4.F + 0.333333F;
  auto column2 = width - column1;

  auto y = 20.F;
  auto dy = 15.F;

  addInput(Jack::input(slug, module, column1, y, Cubic::ACoefficientCvInput));
  addParam(Knob::small(slug, module, column2, y, Cubic::ACoefficientKnob));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Cubic::BCoefficientCvInput));
  addParam(Knob::small(slug, module, column2, y, Cubic::BCoefficientKnob));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Cubic::CCoefficientCvInput));
  addParam(Knob::small(slug, module, column2, y, Cubic::CCoefficientKnob));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Cubic::DCoefficientCvInput));
  addParam(Knob::small(slug, module, column2, y, Cubic::DCoefficientKnob));

  y = 82.F;
  addParam(Knob::small(slug, module, column1, y, Cubic::InputGainKnob));
  addParam(Knob::small(slug, module, column2, y, Cubic::OutputGainKnob));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Cubic::InputGainCvInput));
  addInput(Jack::input(slug, module, column2, y, Cubic::OutputGainCvInput));

  y += dy;
  addInput(Jack::input(slug, module, column1, y, Cubic::CubicInput));
  addOutput(Jack::output(slug, module, column2, y, Cubic::CubicOutput));
}
} // namespace dhe
