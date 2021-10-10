#pragma once

#include "./fuzzy-logic-controls.h"
#include "controls/port.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace fuzzy_logic {
class Panel : public rack::app::ModuleWidget {
  using Controls = FuzzyLogicControls;

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
    Input::install(this, Controls::AInputs + 0, ab_outer_column, y);
    Button::install<Toggle>(this, Controls::NotAButtons + 0, ab_button_column,
                            y);
    Button::install<Toggle>(this, Controls::NotAButtons + 1, cd_button_column,
                            y);
    Input::install(this, Controls::AInputs + 1, cd_outer_column, y);

    y = top + 0.5F * dy;
    ThumbSwitch<2>::install(this, Controls::LevelRangeSwitch, switch_column, y);

    y = top + 1.F * dy;
    Input::install(this, Controls::BInputs + 0, ab_outer_column, y);
    Button::install<Toggle>(this, Controls::NotBButtons + 0, ab_button_column,
                            y);
    Button::install<Toggle>(this, Controls::NotBButtons + 1, cd_button_column,
                            y);
    Input::install(this, Controls::BInputs + 1, cd_outer_column, y);

    y = top + 2.F * dy;
    Output::install(this, Controls::AndOutputs + 0, ab_outer_column, y);
    Output::install(this, Controls::NandOutputs + 0, ab_inner_column, y);
    Output::install(this, Controls::NandOutputs + 1, cd_inner_column, y);
    Output::install(this, Controls::AndOutputs + 1, cd_outer_column, y);

    y = top + 3.F * dy;
    Output::install(this, Controls::OrOutputs + 0, ab_outer_column, y);
    Output::install(this, Controls::NorOutputs + 0, ab_inner_column, y);
    Output::install(this, Controls::NorOutputs + 1, cd_inner_column, y);
    Output::install(this, Controls::OrOutputs + 1, cd_outer_column, y);

    y = top + 4.F * dy;
    Output::install(this, Controls::XorOutputs + 0, ab_outer_column, y);
    Output::install(this, Controls::XnorOutputs + 0, ab_inner_column, y);
    Output::install(this, Controls::XnorOutputs + 1, cd_inner_column, y);
    Output::install(this, Controls::XorOutputs + 1, cd_outer_column, y);

    y = top + 5.F * dy;
    Output::install(this, Controls::ImplicationOutputs + 0, ab_outer_column, y);
    Output::install(this, Controls::NonimplicationOutputs + 0, ab_inner_column,
                    y);
    Output::install(this, Controls::NonimplicationOutputs + 1, cd_inner_column,
                    y);
    Output::install(this, Controls::ImplicationOutputs + 1, cd_outer_column, y);

    y = top + 6.F * dy;
    Output::install(this, Controls::ConverseImplicationOutputs + 0,
                    ab_outer_column, y);
    Output::install(this, Controls::ConverseNonimplicationOutputs + 0,
                    ab_inner_column, y);
    Output::install(this, Controls::ConverseNonimplicationOutputs + 1,
                    cd_inner_column, y);
    Output::install(this, Controls::ConverseImplicationOutputs + 1,
                    cd_outer_column, y);
  }
};
} // namespace fuzzy_logic
} // namespace dhe
