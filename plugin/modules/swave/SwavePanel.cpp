#include "SwavePanel.h"

#include "Swave.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

SwavePanel::SwavePanel(rack::engine::Module *module) {
  auto constexpr slug = "swave";
  auto constexpr hp = 4;

  setModule(module);
  setPanel(backgroundSvg(slug));
  installScrews(this, hp);

  auto constexpr width = hp2mm(hp);

  auto constexpr x = width / 2.F;

  auto y = 25.F;
  auto dy = 18.5F;

  addParam(Toggle::stepper(2, slug, module, x, y, Swave::ShapeSwitch));

  y += dy;
  addParam(Knob::large(slug, module, x, y, Swave::CurveKnob));

  y += dy;
  addParam(Knob::tiny(slug, module, x, y, Swave::CurveAvKnob));

  y += dy;
  addInput(Jack::input(slug, module, x, y, Swave::CurveCvInput));

  y = 82.F;
  dy = 15.F;

  y += dy;
  addInput(Jack::input(slug, module, x, y, Swave::SwaveInput));

  y += dy;
  addOutput(Jack::output(slug, module, x, y, Swave::SwaveOutput));
}
} // namespace dhe
