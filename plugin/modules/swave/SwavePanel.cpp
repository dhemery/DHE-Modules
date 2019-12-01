#include "SwavePanel.h"

#include "Swave.h"

namespace dhe {

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
  knob<LargeKnob>(x, y, Swave::CurveKnob);

  y += dy;
  knob<TinyKnob>(x, y, Swave::CurveAvKnob);

  y += dy;
  input(x, y, Swave::CurveCvInput);

  y = 82.F;
  dy = 15.F;

  y += dy;
  input(x, y, Swave::SwaveInput);

  y += dy;
  output(x, y, Swave::SwaveOutput);
}
} // namespace dhe
