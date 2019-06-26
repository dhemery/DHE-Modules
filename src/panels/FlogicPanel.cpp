#include "panels/FlogicPanel.h"

namespace dhe {

FlogicPanel::FlogicPanel(Flogic *zadeh) : Panel{zadeh, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto top = hp2mm(4.75F);
  auto const dy = hp2mm(4.F);

  auto y = top + 0.F * dy;
  input(left, y, Flogic::AInput);
  button<ToggleButton>(lc, y, Flogic::NotAButton);
  button<ToggleButton>(rc, y, Flogic::NotBButton);
  input(right, y, Flogic::BInput);

  y = top + 1.F * dy;
  output(left, y, Flogic::AndOutput);
  output(lc, y, Flogic::NandOutput);
  output(rc, y, Flogic::NorOutput);
  output(right, y, Flogic::OrOutput);

  y = top + 2.F * dy;
  output(left, y, Flogic::NotAOutput);
  output(lc, y, Flogic::NotBOutput);
  output(rc, y, Flogic::XnorOutput);
  output(right, y, Flogic::XorOutput);

  y = top + 4.F * dy;
  output(left, y, Flogic::PAndOutput);
  output(lc, y, Flogic::PNandOutput);
  output(rc, y, Flogic::PNorOutput);
  output(right, y, Flogic::POrOutput);

  y = top + 3.F * dy;
  output(left, y, Flogic::AImpliesBOutput);
  output(lc, y, Flogic::ANotImpliesBOutput);
  output(rc, y, Flogic::BNotImpliesAOutput);
  output(right, y, Flogic::BImpliesAOutput);
}
} // namespace dhe
