#include "panels/FuzzyLogicZPanel.h"

namespace dhe {

FuzzyLogicZPanel::FuzzyLogicZPanel(FuzzyLogicZ *fuzzy) : Panel{fuzzy, hp} {
  auto const abOuterColumn = hp2mm(1.5F);
  auto const abButtonColumn = hp2mm(3.F);
  auto const abInnerColumn = hp2mm(3.25F);
  auto const switchColumn = hp2mm(4.5F);
  auto const cdInnerColumn = hp2mm(5.75F);
  auto const cdButtonColumn = hp2mm(6.F);
  auto const cdOuterColumn = hp2mm(7.5F);

  auto const top = hp2mm(4.F);
  auto const dy = hp2mm(3.F);

  auto y = top + 0.F * dy;
  input(abOuterColumn, y, FuzzyLogicZ::AInputs + 0);
  button<ToggleButton>(abButtonColumn, y, FuzzyLogicZ::NotAButtons + 0);
  button<ToggleButton>(cdButtonColumn, y, FuzzyLogicZ::NotAButtons + 1);
  input(cdOuterColumn, y, FuzzyLogicZ::AInputs + 1);

  y = top + 0.5F * dy;
  toggle<2>(switchColumn, y, FuzzyLogicZ::LevelRangeSwitch);

  y = top + 1.F * dy;
  input(abOuterColumn, y, FuzzyLogicZ::BInputs + 0);
  button<ToggleButton>(abButtonColumn, y, FuzzyLogicZ::NotBButtons + 0);
  button<ToggleButton>(cdButtonColumn, y, FuzzyLogicZ::NotBButtons + 1);
  input(cdOuterColumn, y, FuzzyLogicZ::BInputs + 1);

  y = top + 2.F * dy;
  output(abOuterColumn, y, FuzzyLogicZ::AndOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicZ::NandOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicZ::NandOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicZ::AndOutputs + 1);

  y = top + 3.F * dy;
  output(abOuterColumn, y, FuzzyLogicZ::OrOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicZ::NorOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicZ::XnorOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicZ::XorOutputs + 1);

  y = top + 4.F * dy;
  output(abOuterColumn, y, FuzzyLogicZ::XorOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicZ::XnorOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicZ::XnorOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicZ::XorOutputs + 1);

  y = top + 5.F * dy;
  output(abOuterColumn, y, FuzzyLogicZ::ImplicationOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicZ::NonimplicationOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicZ::NonimplicationOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicZ::ImplicationOutputs + 1);

  y = top + 6.F * dy;
  output(abOuterColumn, y, FuzzyLogicZ::ConverseImplicationOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicZ::ConverseNonimplicationOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicZ::ConverseNonimplicationOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicZ::ConverseImplicationOutputs + 1);
}
} // namespace dhe
