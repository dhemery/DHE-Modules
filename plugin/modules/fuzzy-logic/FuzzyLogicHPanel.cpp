#include "FuzzyLogicHPanel.h"

#include "FuzzyLogicH.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

FuzzyLogicHPanel::FuzzyLogicHPanel(rack::engine::Module *module) {
  auto constexpr hp = 9;

  setModule(module);
  setPanel(backgroundSvg(moduleSlug));
  installScrews(this, hp);

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
  addInput(Jack::input(moduleSlug, module, abOuterColumn, y, FuzzyLogicH::AInputs + 0));
  addParam(Toggle::button(moduleSlug, module, abButtonColumn, y, FuzzyLogicH::NotAButtons + 0));
  addParam(Toggle::button(moduleSlug, module, cdButtonColumn, y, FuzzyLogicH::NotAButtons + 1));
  addInput(Jack::input(moduleSlug, module, cdOuterColumn, y, FuzzyLogicH::AInputs + 1));

  y = top + 0.5F * dy;
  addParam(Toggle::stepper(2, moduleSlug, module, switchColumn, y, FuzzyLogicH::LevelRangeSwitch));

  y = top + 1.F * dy;
  addInput(Jack::input(moduleSlug, module, abOuterColumn, y, FuzzyLogicH::BInputs + 0));
  addParam(Toggle::button(moduleSlug, module, abButtonColumn, y, FuzzyLogicH::NotBButtons + 0));
  addParam(Toggle::button(moduleSlug, module, cdButtonColumn, y, FuzzyLogicH::NotBButtons + 1));
  addInput(Jack::input(moduleSlug, module, cdOuterColumn, y, FuzzyLogicH::BInputs + 1));

  y = top + 2.F * dy;
  addOutput(Jack::output(moduleSlug, module, abOuterColumn, y, FuzzyLogicH::AndOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, abInnerColumn, y, FuzzyLogicH::NandOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, cdInnerColumn, y, FuzzyLogicH::NandOutputs + 1));
  addOutput(Jack::output(moduleSlug, module, cdOuterColumn, y, FuzzyLogicH::AndOutputs + 1));

  y = top + 3.F * dy;
  addOutput(Jack::output(moduleSlug, module, abOuterColumn, y, FuzzyLogicH::OrOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, abInnerColumn, y, FuzzyLogicH::NorOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, cdInnerColumn, y, FuzzyLogicH::NorOutputs + 1));
  addOutput(Jack::output(moduleSlug, module, cdOuterColumn, y, FuzzyLogicH::OrOutputs + 1));

  y = top + 4.F * dy;
  addOutput(Jack::output(moduleSlug, module, abOuterColumn, y, FuzzyLogicH::XorOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, abInnerColumn, y, FuzzyLogicH::XnorOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, cdInnerColumn, y, FuzzyLogicH::XnorOutputs + 1));
  addOutput(Jack::output(moduleSlug, module, cdOuterColumn, y, FuzzyLogicH::XorOutputs + 1));

  y = top + 5.F * dy;
  addOutput(Jack::output(moduleSlug, module, abOuterColumn, y, FuzzyLogicH::ImplicationOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, abInnerColumn, y, FuzzyLogicH::NonimplicationOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, cdInnerColumn, y, FuzzyLogicH::NonimplicationOutputs + 1));
  addOutput(Jack::output(moduleSlug, module, cdOuterColumn, y, FuzzyLogicH::ImplicationOutputs + 1));

  y = top + 6.F * dy;
  addOutput(Jack::output(moduleSlug, module, abOuterColumn, y, FuzzyLogicH::ConverseImplicationOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, abInnerColumn, y, FuzzyLogicH::ConverseNonimplicationOutputs + 0));
  addOutput(Jack::output(moduleSlug, module, cdInnerColumn, y, FuzzyLogicH::ConverseNonimplicationOutputs + 1));
  addOutput(Jack::output(moduleSlug, module, cdOuterColumn, y, FuzzyLogicH::ConverseImplicationOutputs + 1));
}
} // namespace dhe
