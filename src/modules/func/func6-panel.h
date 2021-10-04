#pragma once

#include "func-controls.h"
#include "switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

class Func6Panel : public rack::app::ModuleWidget {
  static auto constexpr channel_count = 6;
  using Controls = FuncControls<channel_count>;
  using Jack = Jack<Func6Panel>;
  using Knob = Knob<Func6Panel>;
  using OperatorSwitch = OperatorSwitch<Func6Panel>;
  using Switch = Switch<Func6Panel>;

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
      auto const y = top + row * row_spacing;
      auto const port_y = y + port_offset;

      addInput(Jack::input(module, column1, port_y, Controls::FuncInput + row));
      addParam(Knob::large(module, column3, y, Controls::AmountKnob + row));
      addOutput(
          Jack::output(module, column5, port_y, Controls::FuncOutput + row));

      auto *offset_range_stepper = Switch::template create<OffsetRangeStepper>(
          module, column4, y, Controls::OffsetRangeSwitch + row);
      addParam(offset_range_stepper);

      auto *multiplier_range_stepper =
          Switch::template create<MultiplierRangeStepper>(
              module, column4, y, Controls::MultiplierRangeSwitch + row);
      addParam(multiplier_range_stepper);

      auto const update_range_stepper =
          [offset_range_stepper, multiplier_range_stepper](bool is_multiply) {
            if (is_multiply) {
              offset_range_stepper->hide();
              multiplier_range_stepper->show();
            } else {
              offset_range_stepper->show();
              multiplier_range_stepper->hide();
            }
          };

      addParam(OperatorSwitch::create(module, column2, y,
                                      Controls::OperationSwitch + row,
                                      update_range_stepper));
    }
  }
}; // namespace func
} // namespace func
} // namespace dhe
