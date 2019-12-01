#pragma once

#include "FuncControls.h"
#include "RangeSteppers.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

  class Func6Panel : public rack::app::ModuleWidget {
  public:
    explicit Func6Panel(rack::engine::Module *module) {
      auto constexpr hp = 12;

      setModule(module);
      setPanel(backgroundSvg(moduleSlug));
      installScrews(this, hp);

      auto width = hp2mm(hp);

      auto const column3 = width / 2.F;
      auto const column1 = width / 7.F;
      auto const column5 = width - column1;
      auto const column2 = (column3 - column1) / 2.F + column1;
      auto const column4 = width - column2;

      auto constexpr top = 23.F;
      auto constexpr bottom = 108.F;
      auto constexpr rowCount = 6.F;
      auto constexpr rowSpacing = (bottom - top) / (rowCount - 1.F);
      auto constexpr portOffset = 1.25F;

      for (auto row = 0; row < rowCount; row++) {
        auto const y = top + row * rowSpacing;
        auto const portY = y + portOffset;

        installInput(this, module, column1, portY, FuncControls<6>::FuncInput + row);
        addParam(Knob::large(moduleSlug, module, column3, y, FuncControls<6>::AmountKnob + row));
        installOutput(this, module, column5, portY, FuncControls<6>::FuncOutput + row);

        auto additionRangeStepper
            = install<AdditionRangeStepper>(this, module, column4, y, FuncControls<6>::OffsetRangeSwitch + row);
        auto multiplicationRangeStepper = install<MultiplicationRangeStepper>(
            this, module, column4, y, FuncControls<6>::MultiplierRangeSwitch + row);
        multiplicationRangeStepper->visible = false;

        auto updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
          additionRangeStepper->visible = !isMultiply;
          multiplicationRangeStepper->visible = isMultiply;
        };

        auto operatorSwitch = install<OperatorSwitch>(this, module, column2, y, FuncControls<6>::OperationSwitch + row);

        operatorSwitch->onOperatorChange(updateRangeStepperVisibility);
      }
    }
    static constexpr auto moduleSlug = "func-6";
  };
} // namespace func
} // namespace dhe
