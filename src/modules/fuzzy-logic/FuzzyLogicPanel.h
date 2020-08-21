#pragma once

#include "FuzzyLogicControls.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace fuzzy_logic {
class FuzzyLogicPanel : public rack::app::ModuleWidget {
  using Controls = FuzzyLogicControls;

public:
  explicit FuzzyLogicPanel(std::string const &slug,
                           rack::engine::Module *module) {
    auto constexpr hp = 9;

    setModule(module);
    setPanel(background_svg(slug));
    install_screws(this, hp);

    auto constexpr abOuterColumn = hp2mm(1.5F);
    auto constexpr abButtonColumn = hp2mm(3.F);
    auto constexpr abInnerColumn = hp2mm(3.25F);
    auto constexpr switchColumn = hp2mm(4.5F);
    auto constexpr cdInnerColumn = hp2mm(5.75F);
    auto constexpr cdButtonColumn = hp2mm(6.F);
    auto constexpr cdOuterColumn = hp2mm(7.5F);

    auto constexpr top = hp2mm(4.F);
    auto constexpr dy = hp2mm(3.F);

    auto y = top + 0.F * dy;
    addInput(
        Jack::input(slug, module, abOuterColumn, y, Controls::AInputs + 0));
    addParam(Toggle::button(slug, module, abButtonColumn, y,
                            Controls::NotAButtons + 0));
    addParam(Toggle::button(slug, module, cdButtonColumn, y,
                            Controls::NotAButtons + 1));
    addInput(
        Jack::input(slug, module, cdOuterColumn, y, Controls::AInputs + 1));

    y = top + 0.5F * dy;
    addParam(Toggle::stepper(2, slug, module, switchColumn, y,
                             Controls::LevelRangeSwitch));

    y = top + 1.F * dy;
    addInput(
        Jack::input(slug, module, abOuterColumn, y, Controls::BInputs + 0));
    addParam(Toggle::button(slug, module, abButtonColumn, y,
                            Controls::NotBButtons + 0));
    addParam(Toggle::button(slug, module, cdButtonColumn, y,
                            Controls::NotBButtons + 1));
    addInput(
        Jack::input(slug, module, cdOuterColumn, y, Controls::BInputs + 1));

    y = top + 2.F * dy;
    addOutput(
        Jack::output(slug, module, abOuterColumn, y, Controls::AndOutputs + 0));
    addOutput(Jack::output(slug, module, abInnerColumn, y,
                           Controls::NandOutputs + 0));
    addOutput(Jack::output(slug, module, cdInnerColumn, y,
                           Controls::NandOutputs + 1));
    addOutput(
        Jack::output(slug, module, cdOuterColumn, y, Controls::AndOutputs + 1));

    y = top + 3.F * dy;
    addOutput(
        Jack::output(slug, module, abOuterColumn, y, Controls::OrOutputs + 0));
    addOutput(
        Jack::output(slug, module, abInnerColumn, y, Controls::NorOutputs + 0));
    addOutput(
        Jack::output(slug, module, cdInnerColumn, y, Controls::NorOutputs + 1));
    addOutput(
        Jack::output(slug, module, cdOuterColumn, y, Controls::OrOutputs + 1));

    y = top + 4.F * dy;
    addOutput(
        Jack::output(slug, module, abOuterColumn, y, Controls::XorOutputs + 0));
    addOutput(Jack::output(slug, module, abInnerColumn, y,
                           Controls::XnorOutputs + 0));
    addOutput(Jack::output(slug, module, cdInnerColumn, y,
                           Controls::XnorOutputs + 1));
    addOutput(
        Jack::output(slug, module, cdOuterColumn, y, Controls::XorOutputs + 1));

    y = top + 5.F * dy;
    addOutput(Jack::output(slug, module, abOuterColumn, y,
                           Controls::ImplicationOutputs + 0));
    addOutput(Jack::output(slug, module, abInnerColumn, y,
                           Controls::NonimplicationOutputs + 0));
    addOutput(Jack::output(slug, module, cdInnerColumn, y,
                           Controls::NonimplicationOutputs + 1));
    addOutput(Jack::output(slug, module, cdOuterColumn, y,
                           Controls::ImplicationOutputs + 1));

    y = top + 6.F * dy;
    addOutput(Jack::output(slug, module, abOuterColumn, y,
                           Controls::ConverseImplicationOutputs + 0));
    addOutput(Jack::output(slug, module, abInnerColumn, y,
                           Controls::ConverseNonimplicationOutputs + 0));
    addOutput(Jack::output(slug, module, cdInnerColumn, y,
                           Controls::ConverseNonimplicationOutputs + 1));
    addOutput(Jack::output(slug, module, cdOuterColumn, y,
                           Controls::ConverseImplicationOutputs + 1));
  }
};
} // namespace fuzzy_logic
} // namespace dhe
