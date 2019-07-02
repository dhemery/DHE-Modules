#include "panels/FuzzyLogicHPanel.h"

namespace dhe {

FuzzyLogicHPanel::FuzzyLogicHPanel(FuzzyLogicH *fuzzy) : Panel{fuzzy, hp} {
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
  input(abOuterColumn, y, FuzzyLogicH::AInputs + 0);
  button<ToggleButton>(abButtonColumn, y, FuzzyLogicH::NotAButtons + 0);
  button<ToggleButton>(cdButtonColumn, y, FuzzyLogicH::NotAButtons + 1);
  input(cdOuterColumn, y, FuzzyLogicH::AInputs + 1);

  y = top + 0.5F * dy;
  toggle<2>(switchColumn, y, FuzzyLogicH::LevelRangeSwitch);

  y = top + 1.F * dy;
  input(abOuterColumn, y, FuzzyLogicH::BInputs + 0);
  button<ToggleButton>(abButtonColumn, y, FuzzyLogicH::NotBButtons + 0);
  button<ToggleButton>(cdButtonColumn, y, FuzzyLogicH::NotBButtons + 1);
  input(cdOuterColumn, y, FuzzyLogicH::BInputs + 1);

  y = top + 2.F * dy;
  output(abOuterColumn, y, FuzzyLogicH::AndOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicH::NandOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicH::NandOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicH::AndOutputs + 1);

  y = top + 3.F * dy;
  output(abOuterColumn, y, FuzzyLogicH::OrOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicH::NorOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicH::NorOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicH::OrOutputs + 1);

  y = top + 4.F * dy;
  output(abOuterColumn, y, FuzzyLogicH::XorOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicH::XnorOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicH::XnorOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicH::XorOutputs + 1);

  y = top + 5.F * dy;
  output(abOuterColumn, y, FuzzyLogicH::ImplicationOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicH::NonimplicationOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicH::NonimplicationOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicH::ImplicationOutputs + 1);

  y = top + 6.F * dy;
  output(abOuterColumn, y, FuzzyLogicH::ConverseImplicationOutputs + 0);
  output(abInnerColumn, y, FuzzyLogicH::ConverseNonimplicationOutputs + 0);
  output(cdInnerColumn, y, FuzzyLogicH::ConverseNonimplicationOutputs + 1);
  output(cdOuterColumn, y, FuzzyLogicH::ConverseImplicationOutputs + 1);
}
} // namespace dhe
