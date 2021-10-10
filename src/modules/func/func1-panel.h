#pragma once

#include "./func-controls.h"
#include "./switches.h"
#include "controls/port.h"
#include "widgets/knobs.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

class Func1Panel : public rack::app::ModuleWidget {
  static auto constexpr channel_count = 1;
  using Controls = FuncControls<channel_count>;

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

    Input::install(this, Controls::FuncInput, x, row1);
    Knob::install<Large>(this, Controls::AmountKnob, x, row3);
    Output::install(this, Controls::FuncOutput, x, row6);

    auto *offset_range_stepper = Stepper<OffsetRanges>::install(
        this, Controls::OffsetRangeSwitch, x, row4);

    auto *multiplier_range_stepper = Stepper<MultiplierRanges>::install(
        this, Controls::MultiplierRangeSwitch, x, row4);

    auto const update_range_stepper_visibility =
        [offset_range_stepper, multiplier_range_stepper](Operation op) {
          if (op == Operation::Multiply) {
            offset_range_stepper->hide();
            multiplier_range_stepper->show();
          } else {
            offset_range_stepper->show();
            multiplier_range_stepper->hide();
          }
        };
    Stepper<Operators>::install(this, Controls::OperationSwitch, x, row2,
                                update_range_stepper_visibility);
  }
};
} // namespace func
} // namespace dhe
