#include "panels/FlogicPanel.h"

namespace dhe {

FlogicPanel::FlogicPanel(Flogic *gator) : Panel{gator, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto const dy = hp2mm(3.F);
  auto top = hp2mm(4.5F);

  auto y = top + 0.F * dy;
  input(left, y, Flogic::AInputs + 0);
  button<ToggleButton>(lc, y, Flogic::NegateAButtons + 0);
  button<ToggleButton>(rc, y, Flogic::NegateBButtons + 0);
  input(right, y, Flogic::BInputs + 0);

  y = top + 1.F * dy;
  output(left, y, Flogic::AndOutputs + 0);
  output(lc, y, Flogic::NegatedAndOutputs + 0);
  output(rc, y, Flogic::OrOutputs + 0);
  output(right, y, Flogic::NegatedOrOutputs + 0);

  y = top + 2.F * dy;
  output(left, y, Flogic::MaxOutputs + 0);
  output(lc, y, Flogic::NegatedMaxOutputs + 0);
  output(rc, y, Flogic::NegatedMinOutputs + 0);
  output(right, y, Flogic::MinOutputs + 0);

  top += hp2mm(10.F);

  y = top + 0.F * dy;
  input(left, y, Flogic::AInputs + 1);
  button<ToggleButton>(lc, y, Flogic::NegateAButtons + 1);
  button<ToggleButton>(rc, y, Flogic::NegateBButtons + 1);
  input(right, y, Flogic::BInputs + 1);

  y = top + 1.F * dy;
  output(left, y, Flogic::AndOutputs + 1);
  output(lc, y, Flogic::NegatedAndOutputs + 1);
  output(rc, y, Flogic::NegatedOrOutputs + 1);
  output(right, y, Flogic::OrOutputs + 1);

  y = top + 2.F * dy;
  output(left, y, Flogic::MaxOutputs + 1);
  output(lc, y, Flogic::NegatedMaxOutputs + 1);
  output(rc, y, Flogic::MinOutputs + 1);
  output(right, y, Flogic::NegatedMinOutputs + 1);
}
} // namespace dhe
