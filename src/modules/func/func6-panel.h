#pragma once

#include "control-ids.h"
#include "params.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"

#include "rack.hpp"

namespace dhe {
namespace func {

struct Func6Panel : public PanelWidget<Func6Panel> {
  static auto constexpr hp = 12;
  static auto constexpr channel_count = 6;
  static auto constexpr panel_file = "func-6";
  static auto constexpr svg_dir = "func";

public:
  Func6Panel(rack::engine::Module *module) : PanelWidget<Func6Panel>{module} {
    using Input = InputIds<channel_count>;
    using Param = ParamIds<channel_count>;
    using Output = OutputIds<channel_count>;
    auto constexpr width = hp2mm(hp);

    auto constexpr column3 = width / 2.F;
    auto constexpr column1 = width / 7.F;
    auto constexpr column5 = width - column1;
    auto constexpr column2 = (column3 - column1) / 2.F + column1;
    auto constexpr column4 = width - column2;

    auto constexpr top = 23.F;
    auto constexpr bottom = 108.F;
    auto constexpr row_spacing = (bottom - top) / (channel_count - 1.F);
    auto constexpr port_offset = 1.25F;

    for (auto row = 0; row < channel_count; row++) {
      auto const y = top + static_cast<float>(row) * row_spacing;
      auto const port_y = y + port_offset;

      InPort::install(this, Input::FuncInput + row, column1, port_y);
      Knob::install<Large>(this, Param::AmountKnob + row, column3, y);

      OutPort::install(this, Output::FuncOutput + row, column5, port_y);

      auto *offset_range_stepper = Stepper<OffsetRanges>::install(
          this, Param::OffsetRangeSwitch + row, column4, y);

      auto *multiplier_range_stepper = Stepper<MultiplierRanges>::install(
          this, Param::MultiplierRangeSwitch + row, column4, y);

      auto const update_range_stepper =
          [offset_range_stepper, multiplier_range_stepper](Operation op) {
            if (op == Operation::Multiply) {
              offset_range_stepper->hide();
              multiplier_range_stepper->show();
            } else {
              offset_range_stepper->show();
              multiplier_range_stepper->hide();
            }
          };
      Stepper<Operators>::install(this, Param::OperationSwitch + row, column2,
                                  y, update_range_stepper);
    }
  }
}; // namespace func
} // namespace func
} // namespace dhe
