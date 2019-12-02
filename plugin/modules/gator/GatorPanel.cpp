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
    addInput(Jack::input(moduleSlug, module, left, y, Gator::Inputs + i));
    addParam(Toggle::button(moduleSlug, module, lc, y, Gator::NegateButtons + i));
    addParam(Toggle::button(moduleSlug, module, rc, y, Gator::NegateButtons + i + Gator::inputCount / 2));
    addInput(Jack::input(moduleSlug, module, right, y, Gator::Inputs + i + Gator::inputCount / 2));
    y += dy;
  }

  auto row = top + 8.75 * dy;
  addOutput(Jack::output(moduleSlug, module, left, row, Gator::AndOutput));
  addOutput(Jack::output(moduleSlug, module, lc, row, Gator::NandOutput));
  addOutput(Jack::output(moduleSlug, module, rc, row, Gator::OrOutput));
  addOutput(Jack::output(moduleSlug, module, right, row, Gator::NorOutput));

  row += hp2mm(2.75);
  addOutput(Jack::output(moduleSlug, module, left, row, Gator::EvenOutput));
  addOutput(Jack::output(moduleSlug, module, lc, row, Gator::OddOutput));
  addOutput(Jack::output(moduleSlug, module, rc, row, Gator::XorOutput));
  addOutput(Jack::output(moduleSlug, module, right, row, Gator::XnorOutput));
}
} // namespace dhe
