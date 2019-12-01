#pragma once

#include "FuncControls.h"
#include "RangeSteppers.h"
#include "widgets/Panel.h"

namespace dhe {
namespace func {

  class Func1Panel : public Panel<Func1Panel> {
  public:
    explicit Func1Panel(rack::engine::Module *module) {
      setModule(module);
      setPanel(backgroundSvg<Func1Panel>());
      installScrews(this);

      auto const width = hp2mm(hp);

      auto const x = width / 2.F;

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

      installInput(this, module, x, row1, FuncControls<1>::FuncInput);
      knob<LargeKnob>(x, row3, FuncControls<1>::AmountKnob);
      installOutput(this, module, x, row6, FuncControls<1>::FuncOutput);

      auto additionRangeStepper = toggle<AdditionRangeStepper>(x, row4, FuncControls<1>::OffsetRangeSwitch);
      auto multiplicationRangeStepper
          = toggle<MultiplicationRangeStepper>(x, row4, FuncControls<1>::MultiplierRangeSwitch);
      multiplicationRangeStepper->visible = false;

      auto operatorSwitch = toggle<OperatorSwitch>(x, row2, FuncControls<1>::OperationSwitch);
      auto updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
        additionRangeStepper->visible = !isMultiply;
        multiplicationRangeStepper->visible = isMultiply;
      };
      operatorSwitch->onOperatorChange(updateRangeStepperVisibility);
    }

    static constexpr auto moduleSlug = "func";
    static constexpr auto hp = 3;
  };
} // namespace func
} // namespace dhe
