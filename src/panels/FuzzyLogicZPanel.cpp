#include "panels/FuzzyLogicZPanel.h"

namespace dhe {

FuzzyLogicZPanel::FuzzyLogicZPanel(FuzzyLogicZ *fuzzy) : Panel{fuzzy, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto const top = hp2mm(4.F);
  auto const dy = hp2mm(3.F);

  auto y = top + 0.F * dy;
  input(left, y, FuzzyLogicZ::AInputs + 0);
  button<ToggleButton>(lc, y, FuzzyLogicZ::NotAButtons + 0);
  button<ToggleButton>(rc, y, FuzzyLogicZ::NotAButtons + 1);
  input(right, y, FuzzyLogicZ::AInputs + 1);

  y = top + 1.F * dy;
  input(left, y, FuzzyLogicZ::BInputs + 0);
  button<ToggleButton>(lc, y, FuzzyLogicZ::NotBButtons + 0);
  button<ToggleButton>(rc, y, FuzzyLogicZ::NotBButtons + 1);
  input(right, y, FuzzyLogicZ::BInputs + 1);

  y = top + 2.F * dy;
  output(left, y, FuzzyLogicZ::AndOutputs + 0);
  output(lc, y, FuzzyLogicZ::NandOutputs + 0);
  output(rc, y, FuzzyLogicZ::NandOutputs + 1);
  output(right, y, FuzzyLogicZ::AndOutputs + 1);

  y = top + 3.F * dy;
  output(left, y, FuzzyLogicZ::OrOutputs + 0);
  output(lc, y, FuzzyLogicZ::NorOutputs + 0);
  output(rc, y, FuzzyLogicZ::NorOutputs + 1);
  output(right, y, FuzzyLogicZ::OrOutputs + 1);

  y = top + 4.F * dy;
  output(left, y, FuzzyLogicZ::XorOutputs + 0);
  output(lc, y, FuzzyLogicZ::XnorOutputs + 0);
  output(rc, y, FuzzyLogicZ::XnorOutputs + 1);
  output(right, y, FuzzyLogicZ::XorOutputs + 1);

  y = top + 5.F * dy;
  output(left, y, FuzzyLogicZ::ImplicationOutputs + 0);
  output(lc, y, FuzzyLogicZ::NonimplicationOutputs + 0);
  output(rc, y, FuzzyLogicZ::NonimplicationOutputs + 1);
  output(right, y, FuzzyLogicZ::ImplicationOutputs + 1);

  y = top + 6.F * dy;
  output(left, y, FuzzyLogicZ::ConverseImplicationOutputs + 0);
  output(lc, y, FuzzyLogicZ::ConverseNonimplicationOutputs + 0);
  output(rc, y, FuzzyLogicZ::ConverseNonimplicationOutputs + 1);
  output(right, y, FuzzyLogicZ::ConverseImplicationOutputs + 1);
}
} // namespace dhe
