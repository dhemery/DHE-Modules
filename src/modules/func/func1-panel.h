#pragma once

#include "control-ids.h"
#include "params.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace func {

struct Func1Panel : public PanelWidget<Func1Panel> {
  static auto constexpr channel_count = 1;

public:
  static auto constexpr hp = 3;
  static auto constexpr panel_file = "func";
  static auto constexpr svg_dir = "func";

  Func1Panel(rack::engine::Module *module) : PanelWidget<Func1Panel>{module} {
    using InputId = InputIds<channel_count>;
    using ParamId = ParamIds<channel_count>;
    using OutputId = OutputIds<channel_count>;

    auto constexpr width = hp2mm(hp);
    auto constexpr x = width / 2.F;

    auto constexpr top = 23.F;
    auto constexpr bottom = 108.F;
    auto constexpr row_count = 6.F;
    auto constexpr row_spacing = (bottom - top) / (row_count - 1.F);
    auto constexpr port_offset = 1.25F;

    auto constexpr row1 = top + port_offset;
    auto constexpr row2 = top + row_spacing;
    auto constexpr row3 = top + row_spacing * 2;
    auto constexpr row4 = top + row_spacing * 3;
    auto constexpr row6 = top + row_spacing * 5 + port_offset;

    InPort::install(this, InputId::Channel, x, row1);
    Knob::install<Large>(this, ParamId::Operand, x, row3);
    OutPort::install(this, OutputId::Channel, x, row6);

    auto *offset_range_stepper =
        Stepper::install<OffsetRanges>(this, ParamId::OffsetRange, x, row4);

    auto *multiplier_range_stepper = Stepper::install<MultiplierRanges>(
        this, ParamId::MultiplierRange, x, row4);

    auto const select_operation =
        [offset_range_stepper,
         multiplier_range_stepper](Operations::Selection selection) {
          auto const is_multiply = selection == Operation::Multiply;
          multiplier_range_stepper->setVisible(is_multiply);
          offset_range_stepper->setVisible(!is_multiply);
        };
    ThumbSwitch::install<Operations>(this, ParamId::Operation, x, row2)
        ->on_change(select_operation);
  }
};
} // namespace func
} // namespace dhe
