#include "panels/FuncPanel.h"
#include "panels/func/FuncSteppers.h"

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

  //  auto channel = &module->channel;
  //  auto addition_range_selector = [channel](int selection) {
  //    channel->set_addition_range(selection);
  //  };
  //
  //  auto multiplication_range_selector = [channel](int selection) {
  //    channel->set_multiplication_range(selection);
  //  };
  //
  //  auto select_operator = [channel, addition_range_switch,
  //                          multiplication_range_switch](int position) {
  //    auto is_multiplication = position == 1;
  //    channel->set_operator(is_multiplication);
  //    multiplication_range_switch->visible = is_multiplication;
  //    addition_range_switch->visible = !is_multiplication;
  //  };

  input(x, row_1, Func::IN);
  toggle<2>(x, row_2, Func::OPERATOR_SWITCH);
  toggle<AdditionRangeStepper>(x, row_4, Func::ADDITION_RANGE_SWITCH);
  toggle<MultiplicationRangeStepper>(x, row_4,
                                     Func::MULTIPLICATION_RANGE_SWITCH);
  knob<LargeKnob>(x, row_3, Func::KNOB);
  output(x, row_6, Func::OUT);
}

} // namespace DHE
