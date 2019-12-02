#pragma once

#include "FuncControls.h"
#include "RangeSteppers.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

  class Func1Panel : public rack::app::ModuleWidget {
  public:
    explicit Func1Panel(rack::engine::Module *module) {
      auto const slug = std::string{"func"};
      auto constexpr hp = 3;

      setModule(module);
      setPanel(backgroundSvg(slug));
      installScrews(this, hp);

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

      addInput(Jack::input(slug, module, x, row1, FuncControls<1>::FuncInput));
      addParam(Knob::large(slug, module, x, row3, FuncControls<1>::AmountKnob));
      addOutput(Jack::output(slug, module, x, row6, FuncControls<1>::FuncOutput));

      auto *additionRangeStepper
          = new AdditionRangeStepper{slug, module, x, row4, FuncControls<1>::OffsetRangeSwitch};
      addParam(additionRangeStepper);
      auto *multiplicationRangeStepper
          = new MultiplicationRangeStepper{slug, module, x, row4, FuncControls<1>::MultiplierRangeSwitch};
      addParam(multiplicationRangeStepper);

      auto updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
        additionRangeStepper->visible = !isMultiply;
        multiplicationRangeStepper->visible = isMultiply;
      };
      addParam(new OperatorSwitch{updateRangeStepperVisibility, slug, module, x, row2,
                                  FuncControls<1>::OperationSwitch});
    }
  };
} // namespace func
} // namespace dhe
