#pragma once

#include "func-controls.h"
#include "operator-switch.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

class Func1Panel : public rack::app::ModuleWidget {
  static auto constexpr channel_count = 1;
  using Controls = FuncControls<channel_count>;

public:
  explicit Func1Panel(rack::engine::Module *module) {
    auto constexpr svg_dir = "func";
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

    addInput(Jack::input(svg_dir, module, x, row1, Controls::FuncInput));
    addParam(Knob::large(svg_dir, module, x, row3, Controls::AmountKnob));
    addOutput(Jack::output(svg_dir, module, x, row6, Controls::FuncOutput));

    auto *offset_range_pick_list =
        Toggle::stepper(svg_dir, "offset-range", offset_ranges.size(), module,
                        x, row4, Controls::OffsetRangeSwitch);
    addParam(offset_range_pick_list);

    auto *multiplier_range_pick_list =
        Toggle::stepper(svg_dir, "multiplier-range", multiplier_ranges.size(),
                        module, x, row4, Controls::MultiplierRangeSwitch);
    addParam(multiplier_range_pick_list);

    auto const update_range_stepper_visibility =
        [offset_range_pick_list, multiplier_range_pick_list](bool is_multiply) {
          if (is_multiply) {
            offset_range_pick_list->hide();
            multiplier_range_pick_list->show();
          } else {
            offset_range_pick_list->show();
            multiplier_range_pick_list->hide();
          }
        };
    addParam(new OperatorSwitch{update_range_stepper_visibility, svg_dir,
                                module, x, row2, Controls::OperationSwitch});
  }
};
} // namespace func
} // namespace dhe
