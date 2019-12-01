#pragma once

#include "FuncControls.h"
#include "RangeSteppers.h"

#include <app/ModuleWidget.hpp>
#include <engine/Module.hpp>

namespace dhe {
namespace func {

  class Func1Panel : public rack::app::ModuleWidget {
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
      install<LargeKnob>(this, module, x, row3, FuncControls<1>::AmountKnob);
      installOutput(this, module, x, row6, FuncControls<1>::FuncOutput);

      auto additionRangeStepper
          = install<AdditionRangeStepper>(this, module, x, row4, FuncControls<1>::OffsetRangeSwitch);
      auto multiplicationRangeStepper
          = install<MultiplicationRangeStepper>(this, module, x, row4, FuncControls<1>::MultiplierRangeSwitch);
      multiplicationRangeStepper->visible = false;

      auto operatorSwitch = install<OperatorSwitch>(this, module, x, row2, FuncControls<1>::OperationSwitch);
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
