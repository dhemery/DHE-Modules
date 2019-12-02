#include "SwavePanel.h"

#include "Swave.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

SwavePanel::SwavePanel(rack::engine::Module *module) {
  auto constexpr hp = 4;

  setModule(module);
  setPanel(backgroundSvg(moduleSlug));
  installScrews(this, hp);

  auto constexpr width = hp2mm(hp);

  auto constexpr x = width / 2.F;

  auto y = 25.F;
  auto dy = 18.5F;

  addParam(Toggle::stepper(2, moduleSlug, module, x, y, Swave::ShapeSwitch));

  y += dy;
  addParam(Knob::large(moduleSlug, module, x, y, Swave::CurveKnob));

  y += dy;
  addParam(Knob::tiny(moduleSlug, module, x, y, Swave::CurveAvKnob));

  y += dy;
  installInput(this, module, x, y, Swave::CurveCvInput);

  y = 82.F;
  dy = 15.F;

  y += dy;
  installInput(this, module, x, y, Swave::SwaveInput);

  y += dy;
  installOutput(this, module, x, y, Swave::SwaveOutput);
}
} // namespace dhe
