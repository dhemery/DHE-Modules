#include "GatorPanel.h"

#include "Gator.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

GatorPanel::GatorPanel(rack::engine::Module *module) {
  auto constexpr hp = 9;

  setModule(module);
  setPanel(backgroundSvg(moduleSlug));
  installScrews(this, hp);

  auto const top = hp2mm(3.5F);
  auto const dy = hp2mm(1.75F);

  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto y = top;
  for (int i = 0; i < Gator::inputCount / 2; i++) {
    installInput(this, module, left, y, Gator::Inputs + i);
    addParam(Button::toggle(moduleSlug, module, lc, y, Gator::NegateButtons + i));
    addParam(Button::toggle(moduleSlug, module, rc, y, Gator::NegateButtons + i + Gator::inputCount / 2));
    installInput(this, module, right, y, Gator::Inputs + i + Gator::inputCount / 2);
    y += dy;
  }

  auto row = top + 8.75 * dy;
  installOutput(this, module, left, row, Gator::AndOutput);
  installOutput(this, module, lc, row, Gator::NandOutput);
  installOutput(this, module, rc, row, Gator::OrOutput);
  installOutput(this, module, right, row, Gator::NorOutput);

  row += hp2mm(2.75);
  installOutput(this, module, left, row, Gator::EvenOutput);
  installOutput(this, module, lc, row, Gator::OddOutput);
  installOutput(this, module, rc, row, Gator::XorOutput);
  installOutput(this, module, right, row, Gator::XnorOutput);
}
} // namespace dhe
