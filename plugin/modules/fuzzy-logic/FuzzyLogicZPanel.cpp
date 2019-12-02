#include "FuzzyLogicZPanel.h"

#include "FuzzyLogicZ.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

namespace dhe {

FuzzyLogicZPanel::FuzzyLogicZPanel(rack::engine::Module *module) {
  auto const slug = std::string{"fuzzy-logic-z"};
  auto constexpr hp = 9;

  setModule(module);
  setPanel(backgroundSvg(slug));
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
  addInput(Jack::input(slug, module, abOuterColumn, y, FuzzyLogicZ::AInputs + 0));
  addParam(Toggle::button(slug, module, abButtonColumn, y, FuzzyLogicZ::NotAButtons + 0));
  addParam(Toggle::button(slug, module, cdButtonColumn, y, FuzzyLogicZ::NotAButtons + 1));
  addInput(Jack::input(slug, module, cdOuterColumn, y, FuzzyLogicZ::AInputs + 1));

  y = top + 0.5F * dy;
  addParam(Toggle::stepper(2, slug, module, switchColumn, y, FuzzyLogicZ::LevelRangeSwitch));

  y = top + 1.F * dy;
  addInput(Jack::input(slug, module, abOuterColumn, y, FuzzyLogicZ::BInputs + 0));
  addParam(Toggle::button(slug, module, abButtonColumn, y, FuzzyLogicZ::NotBButtons + 0));
  addParam(Toggle::button(slug, module, cdButtonColumn, y, FuzzyLogicZ::NotBButtons + 1));
  addInput(Jack::input(slug, module, cdOuterColumn, y, FuzzyLogicZ::BInputs + 1));

  y = top + 2.F * dy;
  addOutput(Jack::output(slug, module, abOuterColumn, y, FuzzyLogicZ::AndOutputs + 0));
  addOutput(Jack::output(slug, module, abInnerColumn, y, FuzzyLogicZ::NandOutputs + 0));
  addOutput(Jack::output(slug, module, cdInnerColumn, y, FuzzyLogicZ::NandOutputs + 1));
  addOutput(Jack::output(slug, module, cdOuterColumn, y, FuzzyLogicZ::AndOutputs + 1));

  y = top + 3.F * dy;
  addOutput(Jack::output(slug, module, abOuterColumn, y, FuzzyLogicZ::OrOutputs + 0));
  addOutput(Jack::output(slug, module, abInnerColumn, y, FuzzyLogicZ::NorOutputs + 0));
  addOutput(Jack::output(slug, module, cdInnerColumn, y, FuzzyLogicZ::NorOutputs + 1));
  addOutput(Jack::output(slug, module, cdOuterColumn, y, FuzzyLogicZ::OrOutputs + 1));

  y = top + 4.F * dy;
  addOutput(Jack::output(slug, module, abOuterColumn, y, FuzzyLogicZ::XorOutputs + 0));
  addOutput(Jack::output(slug, module, abInnerColumn, y, FuzzyLogicZ::XnorOutputs + 0));
  addOutput(Jack::output(slug, module, cdInnerColumn, y, FuzzyLogicZ::XnorOutputs + 1));
  addOutput(Jack::output(slug, module, cdOuterColumn, y, FuzzyLogicZ::XorOutputs + 1));

  y = top + 5.F * dy;
  addOutput(Jack::output(slug, module, abOuterColumn, y, FuzzyLogicZ::ImplicationOutputs + 0));
  addOutput(Jack::output(slug, module, abInnerColumn, y, FuzzyLogicZ::NonimplicationOutputs + 0));
  addOutput(Jack::output(slug, module, cdInnerColumn, y, FuzzyLogicZ::NonimplicationOutputs + 1));
  addOutput(Jack::output(slug, module, cdOuterColumn, y, FuzzyLogicZ::ImplicationOutputs + 1));

  y = top + 6.F * dy;
  addOutput(Jack::output(slug, module, abOuterColumn, y, FuzzyLogicZ::ConverseImplicationOutputs + 0));
  addOutput(Jack::output(slug, module, abInnerColumn, y, FuzzyLogicZ::ConverseNonimplicationOutputs + 0));
  addOutput(Jack::output(slug, module, cdInnerColumn, y, FuzzyLogicZ::ConverseNonimplicationOutputs + 1));
  addOutput(Jack::output(slug, module, cdOuterColumn, y, FuzzyLogicZ::ConverseImplicationOutputs + 1));
}
} // namespace dhe
