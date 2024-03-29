#pragma once

#include "control-ids.h"
#include "controls.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/panel-widget.h"

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
    using InputId = InputIds<channel_count>;
    using ParamId = ParamIds<channel_count>;
    using OutputId = OutputIds<channel_count>;
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

      InPort::install(this, InputId::Channel + row, column1, port_y);
      OutPort::install(this, OutputId::Channel + row, column5, port_y);

      auto *operand_knob =
          OperandKnob::install<Large>(this, ParamId::Operand + row, column3, y);

      auto *offset_range_stepper = OffsetRangeStepper::install(
          this, ParamId::OffsetRange + row, column4, y, operand_knob);

      auto *multiplier_range_stepper = MultiplierRangeStepper::install(
          this, ParamId::MultiplierRange + row, column4, y);

      OperationSwitch::install(this, ParamId::Operation + row, column2, y)
          ->set_offset_range_stepper(offset_range_stepper)
          ->set_multiplier_range_stepper(multiplier_range_stepper);
    }
  }
}; // namespace func
} // namespace func
} // namespace dhe
