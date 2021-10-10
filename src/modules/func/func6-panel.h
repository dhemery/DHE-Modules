#pragma once

#include "./func-controls.h"
#include "./switches.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/knobs.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

class Func6Panel : public rack::app::ModuleWidget {
  static auto constexpr channel_count = 6;
  using Controls = FuncControls<channel_count>;

public:
  static auto constexpr svg_dir = "func";

  explicit Func6Panel(rack::engine::Module *module) {
    auto constexpr hp = 12;

    setModule(module);
    setPanel(load_svg(svg_dir, "func-6"));
    install_screws(this, hp);

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

      Input::install(this, Controls::FuncInput + row, column1, port_y);
      Knob::install<Large>(this, Controls::AmountKnob + row, column3, y);

      Output::install(this, Controls::FuncOutput + row, column5, port_y);

      auto *offset_range_stepper = Stepper<OffsetRanges>::install(
          this, Controls::OffsetRangeSwitch + row, column4, y);

      auto *multiplier_range_stepper = Stepper<MultiplierRanges>::install(
          this, Controls::MultiplierRangeSwitch + row, column4, y);

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
      Stepper<Operators>::install(this, Controls::OperationSwitch + row,
                                  column2, y, update_range_stepper);
    }
  }
}; // namespace func
} // namespace func
} // namespace dhe
