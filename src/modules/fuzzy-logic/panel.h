#pragma once

#include "fuzzy-logic-controls.h"

#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {

namespace fuzzy_logic {
struct HPanel {
  static auto constexpr svg_name = "fuzzy-logic-h";
};

struct ZPanel {
  static auto constexpr svg_name = "fuzzy-logic-h";
};

template <typename TStyle> struct Panel : public PanelWidget<Panel<TStyle>> {
  using Controls = FuzzyLogicControls;
  static auto constexpr svg_dir = "fuzzy-logic";
  static auto constexpr svg_name = TStyle::svg_name;
  static auto constexpr hp = 9;

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
    InPort::install(this, Controls::AInputs + 0, ab_outer_column, y);
    Button::install<Toggle>(this, Controls::NotAButtons + 0,
                            mm2px(ab_button_column, y));
    Button::install<Toggle>(this, Controls::NotAButtons + 1,
                            mm2px(cd_button_column, y));
    InPort::install(this, Controls::AInputs + 1, cd_outer_column, y);

    y = top + 0.5F * dy;
    ThumbSwitch<2>::install(this, Controls::LevelRangeSwitch, switch_column, y);

    y = top + 1.F * dy;
    InPort::install(this, Controls::BInputs + 0, ab_outer_column, y);
    Button::install<Toggle>(this, Controls::NotBButtons + 0,
                            mm2px(ab_button_column, y));
    Button::install<Toggle>(this, Controls::NotBButtons + 1,
                            mm2px(cd_button_column, y));
    InPort::install(this, Controls::BInputs + 1, cd_outer_column, y);

    y = top + 2.F * dy;
    OutPort::install(this, Controls::AndOutputs + 0, ab_outer_column, y);
    OutPort::install(this, Controls::NandOutputs + 0, ab_inner_column, y);
    OutPort::install(this, Controls::NandOutputs + 1, cd_inner_column, y);
    OutPort::install(this, Controls::AndOutputs + 1, cd_outer_column, y);

    y = top + 3.F * dy;
    OutPort::install(this, Controls::OrOutputs + 0, ab_outer_column, y);
    OutPort::install(this, Controls::NorOutputs + 0, ab_inner_column, y);
    OutPort::install(this, Controls::NorOutputs + 1, cd_inner_column, y);
    OutPort::install(this, Controls::OrOutputs + 1, cd_outer_column, y);

    y = top + 4.F * dy;
    OutPort::install(this, Controls::XorOutputs + 0, ab_outer_column, y);
    OutPort::install(this, Controls::XnorOutputs + 0, ab_inner_column, y);
    OutPort::install(this, Controls::XnorOutputs + 1, cd_inner_column, y);
    OutPort::install(this, Controls::XorOutputs + 1, cd_outer_column, y);

    y = top + 5.F * dy;
    OutPort::install(this, Controls::ImplicationOutputs + 0, ab_outer_column,
                     y);
    OutPort::install(this, Controls::NonimplicationOutputs + 0, ab_inner_column,
                     y);
    OutPort::install(this, Controls::NonimplicationOutputs + 1, cd_inner_column,
                     y);
    OutPort::install(this, Controls::ImplicationOutputs + 1, cd_outer_column,
                     y);

    y = top + 6.F * dy;
    OutPort::install(this, Controls::ConverseImplicationOutputs + 0,
                     ab_outer_column, y);
    OutPort::install(this, Controls::ConverseNonimplicationOutputs + 0,
                     ab_inner_column, y);
    OutPort::install(this, Controls::ConverseNonimplicationOutputs + 1,
                     cd_inner_column, y);
    OutPort::install(this, Controls::ConverseImplicationOutputs + 1,
                     cd_outer_column, y);
  }
};
} // namespace fuzzy_logic
} // namespace dhe
