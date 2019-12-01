#include "SwavePanel.h"

#include "Swave.h"
#include "widgets/Jacks.h"
#include "widgets/Knobs.h"
#include "widgets/Screws.h"

namespace dhe {

using AvKnob = TinyKnob<SwavePanel>;
using MainKnob = LargeKnob<SwavePanel>;

SwavePanel::SwavePanel(rack::engine::Module *module) {
  setModule(module);
  setPanel(backgroundSvg<SwavePanel>());
  installScrews(this);

  auto width = hp2mm(hp);

  auto x = width / 2.F;

  auto y = 25.F;
  auto dy = 18.5F;

  toggle<2>(x, y, Swave::ShapeSwitch);

  y += dy;
  MainKnob::install(this, module, x, y, Swave::CurveKnob);

  y += dy;
  AvKnob::install(this, module, x, y, Swave::CurveAvKnob);

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
