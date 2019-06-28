#include "panels/FuzzyZPanel.h"

namespace dhe {

FuzzyZPanel::FuzzyZPanel(FuzzyZ *fuzzy) : Panel{fuzzy, hp} {
  auto const left = hp2mm(1.5F);
  auto const lc = hp2mm(3.25F);
  auto const rc = hp2mm(5.75F);
  auto const right = hp2mm(7.5F);

  auto const top = hp2mm(4.F);
  auto const dy = hp2mm(3.F);

  auto y = top + 0.F * dy;
  input(left, y, FuzzyZ::AInputs + 0);
  button<ToggleButton>(lc, y, FuzzyZ::NotAButtons + 0);
  button<ToggleButton>(rc, y, FuzzyZ::NotAButtons + 1);
  input(right, y, FuzzyZ::AInputs + 1);

  y = top + 1.F * dy;
  input(left, y, FuzzyZ::BInputs + 0);
  button<ToggleButton>(lc, y, FuzzyZ::NotBButtons + 0);
  button<ToggleButton>(rc, y, FuzzyZ::NotBButtons + 1);
  input(right, y, FuzzyZ::BInputs + 1);

  y = top + 2.F * dy;
  output(left, y, FuzzyZ::AndOutputs + 0);
  output(lc, y, FuzzyZ::NandOutputs + 0);
  output(rc, y, FuzzyZ::NandOutputs + 1);
  output(right, y, FuzzyZ::AndOutputs + 1);

  y = top + 3.F * dy;
  output(left, y, FuzzyZ::OrOutputs + 0);
  output(lc, y, FuzzyZ::NorOutputs + 0);
  output(rc, y, FuzzyZ::XnorOutputs + 1);
  output(right, y, FuzzyZ::XorOutputs + 1);

  y = top + 4.F * dy;
  output(left, y, FuzzyZ::XorOutputs + 0);
  output(lc, y, FuzzyZ::XnorOutputs + 0);
  output(rc, y, FuzzyZ::XnorOutputs + 1);
  output(right, y, FuzzyZ::XorOutputs + 1);

  y = top + 5.F * dy;
  output(left, y, FuzzyZ::ImplicationOutputs + 0);
  output(lc, y, FuzzyZ::NonimplicationOutputs + 0);
  output(rc, y, FuzzyZ::NonimplicationOutputs + 1);
  output(right, y, FuzzyZ::ImplicationOutputs + 1);

  y = top + 6.F * dy;
  output(left, y, FuzzyZ::ConverseImplicationOutputs + 0);
  output(lc, y, FuzzyZ::ConverseNonimplicationOutputs + 0);
  output(rc, y, FuzzyZ::ConverseNonimplicationOutputs + 1);
  output(right, y, FuzzyZ::ConverseImplicationOutputs + 1);
}
} // namespace dhe
