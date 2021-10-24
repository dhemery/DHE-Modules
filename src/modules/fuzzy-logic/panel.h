#pragma once

#include "control-ids.h"

#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/panel-widget.h"

#include "rack.hpp"

namespace dhe {

namespace fuzzy_logic {
struct HPanel {
  static auto constexpr panel_file = "fuzzy-logic-h";
};

struct ZPanel {
  static auto constexpr panel_file = "fuzzy-logic-z";
};

template <typename TStyle> struct Panel : public PanelWidget<Panel<TStyle>> {
  static auto constexpr hp = 9;
  static auto constexpr panel_file = TStyle::panel_file;
  static auto constexpr svg_dir = "fuzzy-logic";

  Panel(rack::engine::Module *module) : PanelWidget<Panel<TStyle>>{module} {
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
    InPort::install(this, Input::AInputs + 0, ab_outer_column, y);
    Button::install<Toggle>(this, Param::NotAButtons + 0, ab_button_column, y);
    Button::install<Toggle>(this, Param::NotAButtons + 1, cd_button_column, y);
    InPort::install(this, Input::AInputs + 1, cd_outer_column, y);

    y = top + 0.5F * dy;
    ThumbSwitch<2>::install(this, Param::LevelRangeSwitch, switch_column, y);

    y = top + 1.F * dy;
    InPort::install(this, Input::BInputs + 0, ab_outer_column, y);
    Button::install<Toggle>(this, Param::NotBButtons + 0, ab_button_column, y);
    Button::install<Toggle>(this, Param::NotBButtons + 1, cd_button_column, y);
    InPort::install(this, Input::BInputs + 1, cd_outer_column, y);

    y = top + 2.F * dy;
    OutPort::install(this, Output::AndOutputs + 0, ab_outer_column, y);
    OutPort::install(this, Output::NandOutputs + 0, ab_inner_column, y);
    OutPort::install(this, Output::NandOutputs + 1, cd_inner_column, y);
    OutPort::install(this, Output::AndOutputs + 1, cd_outer_column, y);

    y = top + 3.F * dy;
    OutPort::install(this, Output::OrOutputs + 0, ab_outer_column, y);
    OutPort::install(this, Output::NorOutputs + 0, ab_inner_column, y);
    OutPort::install(this, Output::NorOutputs + 1, cd_inner_column, y);
    OutPort::install(this, Output::OrOutputs + 1, cd_outer_column, y);

    y = top + 4.F * dy;
    OutPort::install(this, Output::XorOutputs + 0, ab_outer_column, y);
    OutPort::install(this, Output::XnorOutputs + 0, ab_inner_column, y);
    OutPort::install(this, Output::XnorOutputs + 1, cd_inner_column, y);
    OutPort::install(this, Output::XorOutputs + 1, cd_outer_column, y);

    y = top + 5.F * dy;
    OutPort::install(this, Output::ImplicationOutputs + 0, ab_outer_column, y);
    OutPort::install(this, Output::NonimplicationOutputs + 0, ab_inner_column,
                     y);
    OutPort::install(this, Output::NonimplicationOutputs + 1, cd_inner_column,
                     y);
    OutPort::install(this, Output::ImplicationOutputs + 1, cd_outer_column, y);

    y = top + 6.F * dy;
    OutPort::install(this, Output::ConverseImplicationOutputs + 0,
                     ab_outer_column, y);
    OutPort::install(this, Output::ConverseNonimplicationOutputs + 0,
                     ab_inner_column, y);
    OutPort::install(this, Output::ConverseNonimplicationOutputs + 1,
                     cd_inner_column, y);
    OutPort::install(this, Output::ConverseImplicationOutputs + 1,
                     cd_outer_column, y);
  }
};
} // namespace fuzzy_logic
} // namespace dhe
