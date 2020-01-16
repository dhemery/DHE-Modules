#pragma once

#include "FuncControls.h"
#include "RangeSteppers.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace func {

  class Func6Panel : public rack::app::ModuleWidget {
    static auto constexpr channelCount = 6;
    using Controls = FuncControls<channelCount>;

  public:
    explicit Func6Panel(rack::engine::Module *module) {
      auto constexpr slug = "func-6";
      auto constexpr hp = 12;

      setModule(module);
      setPanel(backgroundSvg(slug));
      installScrews(this, hp);

      auto constexpr width = hp2mm(hp);

      auto constexpr column3 = width / 2.F;
      auto constexpr column1 = width / 7.F;
      auto constexpr column5 = width - column1;
      auto constexpr column2 = (column3 - column1) / 2.F + column1;
      auto constexpr column4 = width - column2;

      auto constexpr top = 23.F;
      auto constexpr bottom = 108.F;
      auto constexpr rowSpacing = (bottom - top) / (channelCount - 1.F);
      auto constexpr portOffset = 1.25F;

      for (auto row = 0; row < channelCount; row++) {
        auto const y = top + row * rowSpacing;
        auto const portY = y + portOffset;

        addInput(Jack::input(slug, module, column1, portY, Controls::FuncInput + row));
        addParam(Knob::large(slug, module, column3, y, Controls::AmountKnob + row));
        addOutput(Jack::output(slug, module, column5, portY, Controls::FuncOutput + row));

        auto *additionRangeStepper
            = new AdditionRangeStepper{slug, module, column4, y, Controls::OffsetRangeSwitch + row};
        addParam(additionRangeStepper);
        auto *multiplicationRangeStepper
            = new MultiplicationRangeStepper{slug, module, column4, y, Controls::MultiplierRangeSwitch + row};
        addParam(multiplicationRangeStepper);

        auto const updateRangeStepperVisibility = [additionRangeStepper, multiplicationRangeStepper](bool isMultiply) {
          additionRangeStepper->visible = !isMultiply;
          multiplicationRangeStepper->visible = isMultiply;
        };

        addParam(new OperatorSwitch{updateRangeStepperVisibility, slug, module, column2, y,
                                    Controls::OperationSwitch + row});
      }
    }
  };
} // namespace func
} // namespace dhe
