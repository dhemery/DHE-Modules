#include "panels/Func6Panel.h"
#include "panels/func/FuncSteppers.h"

namespace DHE {
Func6Panel::Func6Panel(Func6 *func6) : Panel{func6, hp} {
  auto widget_right_edge = width();

  auto column_3 = widget_right_edge / 2.f;
  auto column_1 = widget_right_edge / 7.f;
  auto column_5 = widget_right_edge - column_1;
  auto column_2 = (column_3 - column_1) / 2.f + column_1;
  auto column_4 = widget_right_edge - column_2;

  auto top = 23.f;
  auto bottom = 108.f;
  auto row_count = 6;
  auto row_spacing = (bottom - top) / (row_count - 1);
  auto port_offset = 1.25f;

  for (auto row = 0; row < row_count; row++) {
    auto y = top + row * row_spacing;
    auto port_y = y + port_offset;

    auto operator_switch_index = Func6::OPERATOR_SWITCH + row;
    auto addition_range_switch_index = Func6::ADDITION_RANGE_SWITCH + row;
    auto multiplication_range_switch_index =
        Func6::MULTIPLICATION_RANGE_SWITCH + row;

//    auto channel = &func6->channels[row];
    //    auto select_operator = [channel, addition_range_switch,
    //                            multiplication_range_switch](int position) {
    //      auto is_multiplication = position == 1;
    //      channel->set_operator(is_multiplication);
    //      multiplication_range_switch->visible = is_multiplication;
    //      addition_range_switch->visible = !is_multiplication;
    //    };
    //

    input(column_1, port_y, Func6::IN + row);
    toggle<2>(column_2, y, operator_switch_index);
    knob<LargeKnob>(column_3, y, Func6::KNOB + row);
    toggle<MultiplicationRangeStepper>(column_4, y,
                                       multiplication_range_switch_index);
    toggle<AdditionRangeStepper>(column_4, y, addition_range_switch_index);
    output(column_5, port_y, Func6::OUT + row);
  }
}

} // namespace DHE
