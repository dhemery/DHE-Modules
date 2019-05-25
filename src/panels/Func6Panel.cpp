#include "panels/Func6Panel.h"

#include "panels/func/FuncSteppers.h"

namespace dhe {
Func6Panel::Func6Panel(Func6 *func6) : Panel{func6, hp} {
  auto widgetRightEdge = width();

  auto const column3 = widgetRightEdge / 2.F;
  auto const column1 = widgetRightEdge / 7.F;
  auto const column5 = widgetRightEdge - column1;
  auto const column2 = (column3 - column1) / 2.F + column1;
  auto const column4 = widgetRightEdge - column2;

  auto constexpr top = 23.F;
  auto constexpr bottom = 108.F;
  auto constexpr rowCount = 6.F;
  auto constexpr rowSpacing = (bottom - top) / (rowCount - 1.F);
  auto constexpr portOffset = 1.25F;

  for (auto row = 0; row < rowCount; row++) {
    auto const y = top + row * rowSpacing;
    auto const portY = y + portOffset;

    input(column1, portY, Func6::FuncInput + row);
    knob<LargeKnob>(column3, y, Func6::OperandKnob + row);
    output(column5, portY, Func6::FuncOutput + row);

    auto additionRangeStepper = toggle<AdditionRangeStepper>(column4, y, Func6::AdditionRangeSwitch + row);
    auto multiplicationRangeStepper
        = toggle<MultiplicationRangeStepper>(column4, y, Func6::MultiplicationRangeSwitch + row);
    multiplicationRangeStepper->visible = false;

    auto updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
      additionRangeStepper->visible = !isMultiply;
      multiplicationRangeStepper->visible = isMultiply;
    };

    auto operatorSwitch = toggle<OperatorSwitch>(column2, y, Func6::OperationSwitch + row);

    operatorSwitch->onOperatorChange(updateRangeStepperVisibility);
  }
}

} // namespace dhe
