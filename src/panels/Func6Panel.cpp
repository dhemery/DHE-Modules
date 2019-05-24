#include "panels/Func6Panel.h"

#include "panels/func/FuncSteppers.h"

namespace dhe {
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

    input(column_1, port_y, Func6::FuncInput + row);
    knob<LargeKnob>(column_3, y, Func6::OperandKnob + row);
    output(column_5, port_y, Func6::FuncOutput + row);

    auto additionRangeStepper = toggle<AdditionRangeStepper>(column_4, y, Func6::AdditionRangeSwitch + row);
    auto multiplicationRangeStepper
        = toggle<MultiplicationRangeStepper>(column_4, y, Func6::MultiplicationRangeSwitch + row);
    multiplicationRangeStepper->visible = false;

    auto updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
      additionRangeStepper->visible = !isMultiply;
      multiplicationRangeStepper->visible = isMultiply;
    };

    auto operatorSwitch = toggle<OperatorSwitch>(column_2, y, Func6::OperatorSwitch + row);

    operatorSwitch->onOperatorChange(updateRangeStepperVisibility);
  }
}

} // namespace dhe
