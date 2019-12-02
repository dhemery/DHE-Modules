#include "GatorPanel.h"

#include "Gator.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

GatorPanel::GatorPanel(rack::engine::Module *module) {
  auto constexpr slug = "gator";
  auto constexpr hp = 9;

  setModule(module);
  setPanel(backgroundSvg(slug));
  installScrews(this, hp);

  auto constexpr top = hp2mm(3.5F);
  auto constexpr dy = hp2mm(1.75F);

  auto constexpr left = hp2mm(1.5F);
  auto constexpr lc = hp2mm(3.25F);
  auto constexpr rc = hp2mm(5.75F);
  auto constexpr right = hp2mm(7.5F);

  auto y = top;
  for (auto i = 0; i < Gator::inputCount / 2; i++) {
    addInput(Jack::input(slug, module, left, y, Gator::Inputs + i));
    addParam(Toggle::button(slug, module, lc, y, Gator::NegateButtons + i));
    addParam(Toggle::button(slug, module, rc, y, Gator::NegateButtons + i + Gator::inputCount / 2));
    addInput(Jack::input(slug, module, right, y, Gator::Inputs + i + Gator::inputCount / 2));
    y += dy;
  }

  auto row = top + 8.75 * dy;
  addOutput(Jack::output(slug, module, left, row, Gator::AndOutput));
  addOutput(Jack::output(slug, module, lc, row, Gator::NandOutput));
  addOutput(Jack::output(slug, module, rc, row, Gator::OrOutput));
  addOutput(Jack::output(slug, module, right, row, Gator::NorOutput));

  row += hp2mm(2.75);
  addOutput(Jack::output(slug, module, left, row, Gator::EvenOutput));
  addOutput(Jack::output(slug, module, lc, row, Gator::OddOutput));
  addOutput(Jack::output(slug, module, rc, row, Gator::XorOutput));
  addOutput(Jack::output(slug, module, right, row, Gator::XnorOutput));
}
} // namespace dhe
