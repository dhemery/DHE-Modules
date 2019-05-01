#include "panels/FuncPanel.h"
#include "panels/FuncSteppers.h"

namespace DHE {

FuncPanel::FuncPanel(Func *module) : Panel{module, hp} {
  auto widget_right_edge = width();

  auto x = widget_right_edge / 2.f;

  auto top = 23.f;
  auto bottom = 108.f;
  auto row_count = 6;
  auto row_spacing = (bottom - top) / (row_count - 1);
  auto port_offset = 1.25f;

  auto row_1 = top + port_offset;
  auto row_2 = top + row_spacing;
  auto row_3 = top + row_spacing * 2;
  auto row_4 = top + row_spacing * 3;
  auto row_6 = top + row_spacing * 5 + port_offset;

  auto channel = &module->channel;
  auto operator_switch_index = Func::OPERATOR_SWITCH;
  auto addition_range_switch_index = Func::ADDITION_RANGE_SWITCH;
  auto multiplication_range_switch_index = Func::MULTIPLICATION_RANGE_SWITCH;

  auto addition_range_selector = [channel](int selection) {
    channel->set_addition_range(selection);
  };

  auto multiplication_range_selector = [channel](int selection) {
    channel->set_multiplication_range(selection);
  };

  auto multiplication_range_switch = toggle<MultiplicationRangeStepper>(
      multiplication_range_switch_index, 2, multiplication_range_selector);

  auto addition_range_switch = toggle<AdditionRangeStepper>(
      addition_range_switch_index, 1, addition_range_selector);

  auto select_operator = [channel, addition_range_switch,
                          multiplication_range_switch](int position) {
    auto is_multiplication = position == 1;
    channel->set_operator(is_multiplication);
    multiplication_range_switch->visible = is_multiplication;
    addition_range_switch->visible = !is_multiplication;
  };

  auto operator_switch = toggle<2>(operator_switch_index, 0, select_operator);

  install(x, row_1, input(Func::IN));
  install(x, row_2, operator_switch);
  install(x, row_4, addition_range_switch);
  install(x, row_4, multiplication_range_switch);
  install(x, row_3, knob<LargeKnob>(Func::KNOB));
  install(x, row_6, output(Func::OUT));
}

} // namespace DHE
