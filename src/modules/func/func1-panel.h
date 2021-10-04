#pragma once

#include "func-controls.h"
#include "switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

class Func1Panel : public rack::app::ModuleWidget {
  static auto constexpr channel_count = 1;
  using Controls = FuncControls<channel_count>;
  using Jack = Jack<Func1Panel>;
  using Knob = Knob<Func1Panel>;
  using OperatorSwitch = OperatorSwitch<Func1Panel>;
  using Switch = Switch<Func1Panel>;

public:
  static auto constexpr svg_dir = "func";

  explicit Func1Panel(rack::engine::Module *module) {
    auto constexpr hp = 3;

    setModule(module);
    setPanel(load_svg(svg_dir, "func"));
    install_screws(this, hp);

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

    addInput(Jack::input(module, x, row1, Controls::FuncInput));
    addParam(Knob::large(module, x, row3, Controls::AmountKnob));
    addOutput(Jack::output(module, x, row6, Controls::FuncOutput));

    auto *offset_range_stepper = Switch::template create<OffsetRangeStepper>(
        module, x, row4, Controls::OffsetRangeSwitch);
    addParam(offset_range_stepper);

    auto *multiplier_range_stepper =
        Switch::template create<MultiplierRangeStepper>(
            module, x, row4, Controls::MultiplierRangeSwitch);
    addParam(multiplier_range_stepper);

    auto const update_range_stepper_visibility =
        [offset_range_stepper, multiplier_range_stepper](bool is_multiply) {
          if (is_multiply) {
            offset_range_stepper->hide();
            multiplier_range_stepper->show();
          } else {
            offset_range_stepper->show();
            multiplier_range_stepper->hide();
          }
        };
    addParam(OperatorSwitch::create(module, x, row2, Controls::OperationSwitch,
                                    update_range_stepper_visibility));
  }
};
} // namespace func
} // namespace dhe
