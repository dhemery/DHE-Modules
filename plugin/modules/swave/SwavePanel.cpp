#include "SwavePanel.h"

#include "Swave.h"
#include "widgets/Knobs.h"
#include "widgets/Screws.h"
#include "widgets/Toggles.h"

namespace dhe {

SwavePanel::SwavePanel(rack::engine::Module *module) {
  setModule(module);
  setPanel(backgroundSvg<SwavePanel>());
  installScrews(this);

  auto width = hp2mm(hp);

  auto x = width / 2.F;

  auto y = 25.F;
  auto dy = 18.5F;

  install<Toggle, 2>(this, module, x, y, Swave::ShapeSwitch);

  y += dy;
  install<LargeKnob>(this, module, x, y, Swave::CurveKnob);

  y += dy;
  install<TinyKnob>(this, module, x, y, Swave::CurveAvKnob);

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
