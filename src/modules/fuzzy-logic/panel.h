#pragma once

#include "fuzzy-logic-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace fuzzy_logic {
class Panel : public rack::app::ModuleWidget {
  using Controls = FuzzyLogicControls;
  using Jack = Jack<Panel>;
  using Switch = Switch<Panel>;

public:
  static auto constexpr svg_dir = "fuzzy-logic";

  explicit Panel(std::string const &module_name, rack::engine::Module *module) {
    auto constexpr hp = 9;

    setModule(module);
    setPanel(load_svg(svg_dir, module_name));
    install_screws(this, hp);

    auto constexpr ab_outer_column = hp2mm(1.5F);
    auto constexpr ab_button_column = hp2mm(3.F);
    auto constexpr ab_inner_column = hp2mm(3.25F);
    auto constexpr switch_column = hp2mm(4.5F);
    auto constexpr cd_inner_column = hp2mm(5.75F);
    auto constexpr cd_button_column = hp2mm(6.F);
    auto constexpr cd_outer_column = hp2mm(7.5F);

    auto constexpr top = hp2mm(4.F);
    auto constexpr dy = hp2mm(3.F);

    auto y = top + 0.F * dy;
    addInput(Jack::input(module, ab_outer_column, y, Controls::AInputs + 0));
    addParam(
        Switch::toggle(module, ab_button_column, y, Controls::NotAButtons + 0));
    addParam(
        Switch::toggle(module, cd_button_column, y, Controls::NotAButtons + 1));
    addInput(Jack::input(module, cd_outer_column, y, Controls::AInputs + 1));

    y = top + 0.5F * dy;
    addParam(
        Switch::thumb<2>(module, switch_column, y, Controls::LevelRangeSwitch));

    y = top + 1.F * dy;
    addInput(Jack::input(module, ab_outer_column, y, Controls::BInputs + 0));
    addParam(
        Switch::toggle(module, ab_button_column, y, Controls::NotBButtons + 0));
    addParam(
        Switch::toggle(module, cd_button_column, y, Controls::NotBButtons + 1));
    addInput(Jack::input(module, cd_outer_column, y, Controls::BInputs + 1));

    y = top + 2.F * dy;
    addOutput(
        Jack::output(module, ab_outer_column, y, Controls::AndOutputs + 0));
    addOutput(
        Jack::output(module, ab_inner_column, y, Controls::NandOutputs + 0));
    addOutput(
        Jack::output(module, cd_inner_column, y, Controls::NandOutputs + 1));
    addOutput(
        Jack::output(module, cd_outer_column, y, Controls::AndOutputs + 1));

    y = top + 3.F * dy;
    addOutput(
        Jack::output(module, ab_outer_column, y, Controls::OrOutputs + 0));
    addOutput(
        Jack::output(module, ab_inner_column, y, Controls::NorOutputs + 0));
    addOutput(
        Jack::output(module, cd_inner_column, y, Controls::NorOutputs + 1));
    addOutput(
        Jack::output(module, cd_outer_column, y, Controls::OrOutputs + 1));

    y = top + 4.F * dy;
    addOutput(
        Jack::output(module, ab_outer_column, y, Controls::XorOutputs + 0));
    addOutput(
        Jack::output(module, ab_inner_column, y, Controls::XnorOutputs + 0));
    addOutput(
        Jack::output(module, cd_inner_column, y, Controls::XnorOutputs + 1));
    addOutput(
        Jack::output(module, cd_outer_column, y, Controls::XorOutputs + 1));

    y = top + 5.F * dy;
    addOutput(Jack::output(module, ab_outer_column, y,
                           Controls::ImplicationOutputs + 0));
    addOutput(Jack::output(module, ab_inner_column, y,
                           Controls::NonimplicationOutputs + 0));
    addOutput(Jack::output(module, cd_inner_column, y,
                           Controls::NonimplicationOutputs + 1));
    addOutput(Jack::output(module, cd_outer_column, y,
                           Controls::ImplicationOutputs + 1));

    y = top + 6.F * dy;
    addOutput(Jack::output(module, ab_outer_column, y,
                           Controls::ConverseImplicationOutputs + 0));
    addOutput(Jack::output(module, ab_inner_column, y,
                           Controls::ConverseNonimplicationOutputs + 0));
    addOutput(Jack::output(module, cd_inner_column, y,
                           Controls::ConverseNonimplicationOutputs + 1));
    addOutput(Jack::output(module, cd_outer_column, y,
                           Controls::ConverseImplicationOutputs + 1));
  }
};
} // namespace fuzzy_logic
} // namespace dhe
