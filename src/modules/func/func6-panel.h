#pragma once

#include "func-controls.h"
#include "operator-switch.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

class Func6Panel : public rack::app::ModuleWidget {
  static auto constexpr channel_count = 6;
  using Controls = FuncControls<channel_count>;
  using Jack = dhe::Jack<Func6Panel>;

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
      addParam(
          Knob::large(svg_dir, module, column3, y, Controls::AmountKnob + row));
      addOutput(
          Jack::output(module, column5, port_y, Controls::FuncOutput + row));

      auto *offset_range_pick_list =
          Toggle::stepper(svg_dir, "offset-range", offset_ranges.size(), module,
                          column4, y, Controls::OffsetRangeSwitch + row);
      addParam(offset_range_pick_list);

      auto *multiplier_range_pick_list = Toggle::stepper(
          svg_dir, "multiplier-range", multiplier_ranges.size(), module,
          column4, y, Controls::MultiplierRangeSwitch + row);
      addParam(multiplier_range_pick_list);

      auto const update_range_pick_list =
          [offset_range_pick_list,
           multiplier_range_pick_list](bool is_multiply) {
            if (is_multiply) {
              offset_range_pick_list->hide();
              multiplier_range_pick_list->show();
            } else {
              offset_range_pick_list->show();
              multiplier_range_pick_list->hide();
            }
          };

      addParam(new OperatorSwitch{update_range_pick_list, svg_dir, module,
                                  column2, y, Controls::OperationSwitch + row});
    }
  }
}; // namespace func
} // namespace func
} // namespace dhe
