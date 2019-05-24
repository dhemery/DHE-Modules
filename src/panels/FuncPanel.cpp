#include "panels/FuncPanel.h"

#include "panels/func/FuncSteppers.h"

namespace dhe {

FuncPanel::FuncPanel(Func *func) : Panel{func, hp} {
  auto widgetRightEdge = width();

  auto x = widgetRightEdge / 2.F;

  auto top = 23.F;
  auto bottom = 108.F;
  auto rowCount = 6;
  auto rowSpacing = (bottom - top) / (rowCount - 1);
  auto portOffset = 1.25F;

  auto row1 = top + portOffset;
  auto row2 = top + rowSpacing;
  auto row3 = top + rowSpacing * 2;
  auto row4 = top + rowSpacing * 3;
  auto row6 = top + rowSpacing * 5 + portOffset;

  input(x, row1, Func::FuncInput);
  knob<LargeKnob>(x, row3, Func::OperandKnob);
  output(x, row6, Func::FuncOutput);

  auto additionRangeStepper = toggle<AdditionRangeStepper>(x, row4, Func::AdditionRangeSwitch);
  auto multiplicationRangeStepper = toggle<MultiplicationRangeStepper>(x, row4, Func::MultiplicationRangeSwitch);
  multiplicationRangeStepper->visible = false;

  auto operatorSwitch = toggle<OperatorSwitch>(x, row2, Func::OperatorSwitch);
  auto updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
    additionRangeStepper->visible = !isMultiply;
    multiplicationRangeStepper->visible = isMultiply;
  };
  operatorSwitch->onOperatorChange(updateRangeStepperVisibility);
}

} // namespace dhe
