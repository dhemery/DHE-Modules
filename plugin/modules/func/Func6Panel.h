#pragma once

#include "FuncModule.h"
#include "FuncSteppers.h"
#include "widgets/Panel.h"

namespace dhe {
namespace func {

  class Func6Panel : public Panel<Func6Panel> {
  public:
    explicit Func6Panel(FuncModule<6> *func6) : Panel<Func6Panel>{func6, hp} {
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

        input(column1, portY, FuncControls<6>::FuncInput + row);
        knob<LargeKnob>(column3, y, FuncControls<6>::AmountKnob + row);
        output(column5, portY, FuncControls<6>::FuncOutput + row);

        auto additionRangeStepper = toggle<AdditionRangeStepper>(column4, y, FuncControls<6>::OffsetRangeSwitch + row);
        auto multiplicationRangeStepper
            = toggle<MultiplicationRangeStepper>(column4, y, FuncControls<6>::MultiplierRangeSwitch + row);
        multiplicationRangeStepper->visible = false;

        auto updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
          additionRangeStepper->visible = !isMultiply;
          multiplicationRangeStepper->visible = isMultiply;
        };

        auto operatorSwitch = toggle<OperatorSwitch>(column2, y, FuncControls<6>::OperationSwitch + row);

        operatorSwitch->onOperatorChange(updateRangeStepperVisibility);
      }
    }
    static constexpr auto moduleSlug = "func-6";
    static constexpr auto hp = 12;
  };
} // namespace func
} // namespace dhe
