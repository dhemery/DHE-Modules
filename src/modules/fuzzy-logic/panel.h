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
    InPort::install(this, Input::A + 0, ab_outer_column, y);
    Button::install<Toggle>(this, Param::NegateA + 0, ab_button_column, y);
    Button::install<Toggle>(this, Param::NegateA + 1, cd_button_column, y);
    InPort::install(this, Input::A + 1, cd_outer_column, y);

    y = top + 0.5F * dy;
    ThumbSwitch<2>::install(this, Param::LevelRange, switch_column, y);

    y = top + 1.F * dy;
    InPort::install(this, Input::B + 0, ab_outer_column, y);
    Button::install<Toggle>(this, Param::NegateB + 0, ab_button_column, y);
    Button::install<Toggle>(this, Param::NegateB + 1, cd_button_column, y);
    InPort::install(this, Input::B + 1, cd_outer_column, y);

    y = top + 2.F * dy;
    OutPort::install(this, Output::And + 0, ab_outer_column, y);
    OutPort::install(this, Output::Nand + 0, ab_inner_column, y);
    OutPort::install(this, Output::Nand + 1, cd_inner_column, y);
    OutPort::install(this, Output::And + 1, cd_outer_column, y);

    y = top + 3.F * dy;
    OutPort::install(this, Output::Or + 0, ab_outer_column, y);
    OutPort::install(this, Output::Nor + 0, ab_inner_column, y);
    OutPort::install(this, Output::Nor + 1, cd_inner_column, y);
    OutPort::install(this, Output::Or + 1, cd_outer_column, y);

    y = top + 4.F * dy;
    OutPort::install(this, Output::Xor + 0, ab_outer_column, y);
    OutPort::install(this, Output::Xnor + 0, ab_inner_column, y);
    OutPort::install(this, Output::Xnor + 1, cd_inner_column, y);
    OutPort::install(this, Output::Xor + 1, cd_outer_column, y);

    y = top + 5.F * dy;
    OutPort::install(this, Output::Implication + 0, ab_outer_column, y);
    OutPort::install(this, Output::Nonimplication + 0, ab_inner_column, y);
    OutPort::install(this, Output::Nonimplication + 1, cd_inner_column, y);
    OutPort::install(this, Output::Implication + 1, cd_outer_column, y);

    y = top + 6.F * dy;
    OutPort::install(this, Output::ConverseImplication + 0, ab_outer_column, y);
    OutPort::install(this, Output::ConverseNonimplication + 0, ab_inner_column,
                     y);
    OutPort::install(this, Output::ConverseNonimplication + 1, cd_inner_column,
                     y);
    OutPort::install(this, Output::ConverseImplication + 1, cd_outer_column, y);
  }
};
} // namespace fuzzy_logic
} // namespace dhe
