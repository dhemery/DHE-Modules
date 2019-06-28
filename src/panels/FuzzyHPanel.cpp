#include "panels/FuzzyHPanel.h"

namespace dhe {

FuzzyHPanel::FuzzyHPanel(FuzzyH *fuzzy) : Panel{fuzzy, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto const top = hp2mm(4.F);
  auto const dy = hp2mm(3.F);

  auto y = top + 0.F * dy;
  input(left, y, FuzzyH::AInputs + 0);
  button<ToggleButton>(lc, y, FuzzyH::NotAButtons + 0);
  button<ToggleButton>(rc, y, FuzzyH::NotAButtons + 1);
  input(right, y, FuzzyH::AInputs + 1);

  y = top + 1.F * dy;
  input(left, y, FuzzyH::BInputs + 0);
  button<ToggleButton>(lc, y, FuzzyH::NotBButtons + 0);
  button<ToggleButton>(rc, y, FuzzyH::NotBButtons + 1);
  input(right, y, FuzzyH::BInputs + 1);

  y = top + 2.F * dy;
  output(left, y, FuzzyH::AndOutputs + 0);
  output(lc, y, FuzzyH::NandOutputs + 0);
  output(rc, y, FuzzyH::NandOutputs + 1);
  output(right, y, FuzzyH::AndOutputs + 1);

  y = top + 3.F * dy;
  output(left, y, FuzzyH::OrOutputs + 0);
  output(lc, y, FuzzyH::NorOutputs + 0);
  output(rc, y, FuzzyH::XnorOutputs + 1);
  output(right, y, FuzzyH::XorOutputs + 1);

  y = top + 4.F * dy;
  output(left, y, FuzzyH::XorOutputs + 0);
  output(lc, y, FuzzyH::XnorOutputs + 0);
  output(rc, y, FuzzyH::XnorOutputs + 1);
  output(right, y, FuzzyH::XorOutputs + 1);

  y = top + 5.F * dy;
  output(left, y, FuzzyH::ImplicationOutputs + 0);
  output(lc, y, FuzzyH::NonimplicationOutputs + 0);
  output(rc, y, FuzzyH::NonimplicationOutputs + 1);
  output(right, y, FuzzyH::ImplicationOutputs + 1);

  y = top + 6.F * dy;
  output(left, y, FuzzyH::ConverseImplicationOutputs + 0);
  output(lc, y, FuzzyH::ConverseNonimplicationOutputs + 0);
  output(rc, y, FuzzyH::ConverseNonimplicationOutputs + 1);
  output(right, y, FuzzyH::ConverseImplicationOutputs + 1);
}
} // namespace dhe
