#include "panels/Func6Panel.h"

#include "panels/func/FuncSteppers.h"

namespace dhe {
Func6Panel::Func6Panel(Func6 *func6) : Panel{func6, hp} {
  auto widgetRightEdge = width();

  auto column3 = widgetRightEdge / 2.f;
  auto column1 = widgetRightEdge / 7.f;
  auto column5 = widgetRightEdge - column1;
  auto column2 = (column3 - column1) / 2.f + column1;
  auto column4 = widgetRightEdge - column2;

  auto top = 23.f;
  auto bottom = 108.f;
  auto rowCount = 6;
  auto rowSpacing = (bottom - top) / (rowCount - 1);
  auto portOffset = 1.25f;

  for (auto row = 0; row < rowCount; row++) {
    auto y = top + row * rowSpacing;
    auto portY = y + portOffset;

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

    auto operatorSwitch = toggle<OperatorSwitch>(column2, y, Func6::OperatorSwitch + row);

    operatorSwitch->onOperatorChange(updateRangeStepperVisibility);
  }
}

} // namespace dhe
