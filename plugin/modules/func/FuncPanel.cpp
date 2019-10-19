#include "FuncPanel.h"

#include "FuncSteppers.h"

namespace dhe {

FuncPanel::FuncPanel(Func *func) : Panel{func, hp} {
  auto const widgetRightEdge = width();

  auto const x = widgetRightEdge / 2.F;

  auto constexpr top = 23.F;
  auto constexpr bottom = 108.F;
  auto constexpr rowCount = 6.F;
  auto constexpr rowSpacing = (bottom - top) / (rowCount - 1.F);
  auto constexpr portOffset = 1.25F;

  auto constexpr row1 = top + portOffset;
  auto constexpr row2 = top + rowSpacing;
  auto constexpr row3 = top + rowSpacing * 2;
  auto constexpr row4 = top + rowSpacing * 3;
  auto constexpr row6 = top + rowSpacing * 5 + portOffset;

  input(x, row1, Func::FuncInput);
  knob<LargeKnob>(x, row3, Func::AmountKnob);
  output(x, row6, Func::FuncOutput);

  auto additionRangeStepper = toggle<AdditionRangeStepper>(x, row4, Func::OffsetRangeSwitch);
  auto multiplicationRangeStepper = toggle<MultiplicationRangeStepper>(x, row4, Func::MultiplierRangeSwitch);
  multiplicationRangeStepper->visible = false;

  auto operatorSwitch = toggle<OperatorSwitch>(x, row2, Func::OperationSwitch);
  auto updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
    additionRangeStepper->visible = !isMultiply;
    multiplicationRangeStepper->visible = isMultiply;
  };
  operatorSwitch->onOperatorChange(updateRangeStepperVisibility);
}

} // namespace dhe
