#include "panels/FuzzyLogicHPanel.h"

namespace dhe {

FuzzyLogicHPanel::FuzzyLogicHPanel(FuzzyLogicH *fuzzy) : Panel{fuzzy, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto const top = hp2mm(4.F);
  auto const dy = hp2mm(3.F);

  auto y = top + 0.F * dy;
  input(left, y, FuzzyLogicH::AInputs + 0);
  button<ToggleButton>(lc, y, FuzzyLogicH::NotAButtons + 0);
  button<ToggleButton>(rc, y, FuzzyLogicH::NotAButtons + 1);
  input(right, y, FuzzyLogicH::AInputs + 1);

  y = top + 1.F * dy;
  input(left, y, FuzzyLogicH::BInputs + 0);
  button<ToggleButton>(lc, y, FuzzyLogicH::NotBButtons + 0);
  button<ToggleButton>(rc, y, FuzzyLogicH::NotBButtons + 1);
  input(right, y, FuzzyLogicH::BInputs + 1);

  y = top + 2.F * dy;
  output(left, y, FuzzyLogicH::AndOutputs + 0);
  output(lc, y, FuzzyLogicH::NandOutputs + 0);
  output(rc, y, FuzzyLogicH::NandOutputs + 1);
  output(right, y, FuzzyLogicH::AndOutputs + 1);

  y = top + 3.F * dy;
  output(left, y, FuzzyLogicH::OrOutputs + 0);
  output(lc, y, FuzzyLogicH::NorOutputs + 0);
  output(rc, y, FuzzyLogicH::XnorOutputs + 1);
  output(right, y, FuzzyLogicH::XorOutputs + 1);

  y = top + 4.F * dy;
  output(left, y, FuzzyLogicH::XorOutputs + 0);
  output(lc, y, FuzzyLogicH::XnorOutputs + 0);
  output(rc, y, FuzzyLogicH::XnorOutputs + 1);
  output(right, y, FuzzyLogicH::XorOutputs + 1);

  y = top + 5.F * dy;
  output(left, y, FuzzyLogicH::ImplicationOutputs + 0);
  output(lc, y, FuzzyLogicH::NonimplicationOutputs + 0);
  output(rc, y, FuzzyLogicH::NonimplicationOutputs + 1);
  output(right, y, FuzzyLogicH::ImplicationOutputs + 1);

  y = top + 6.F * dy;
  output(left, y, FuzzyLogicH::ConverseImplicationOutputs + 0);
  output(lc, y, FuzzyLogicH::ConverseNonimplicationOutputs + 0);
  output(rc, y, FuzzyLogicH::ConverseNonimplicationOutputs + 1);
  output(right, y, FuzzyLogicH::ConverseImplicationOutputs + 1);
}
} // namespace dhe
