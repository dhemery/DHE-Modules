#include "GatorPanel.h"

namespace dhe {

GatorPanel::GatorPanel(Gator *gator) : Panel{gator, hp} {
  auto const top = hp2mm(3.5F);
  auto const dy = hp2mm(1.75F);

  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto y = top;
  for (int i = 0; i < Gator::inputCount / 2; i++) {
    input(left, y, Gator::Inputs + i);
    button<ToggleButton>(lc, y, Gator::NegateButtons + i);
    button<ToggleButton>(rc, y, Gator::NegateButtons + i + Gator::inputCount / 2);
    input(right, y, Gator::Inputs + i + Gator::inputCount / 2);
    y += dy;
  }

  auto row = top + 8.75 * dy;
  output(left, row, Gator::AndOutput);
  output(lc, row, Gator::NandOutput);
  output(rc, row, Gator::OrOutput);
  output(right, row, Gator::NorOutput);

  row += hp2mm(2.75);
  output(left, row, Gator::EvenOutput);
  output(lc, row, Gator::OddOutput);
  output(rc, row, Gator::XorOutput);
  output(right, row, Gator::XnorOutput);
}
} // namespace dhe
