#pragma once

#include "FuncControls.h"
#include "OperatorSwitch.h"
#include "widgets/PickList.h"

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

      auto popupMenus = std::vector<rack::widget::Widget *>{};

      for (auto row = 0; row < channelCount; row++) {
        auto const y = top + row * rowSpacing;
        auto const portY = y + portOffset;

        addInput(Jack::input(slug, module, column1, portY, Controls::FuncInput + row));
        addParam(Knob::large(slug, module, column3, y, Controls::AmountKnob + row));
        addOutput(Jack::output(slug, module, column5, portY, Controls::FuncOutput + row));

        auto *offsetRangePickList = picklist::button(slug, "offset-range", offsetRanges.size(), module, column4, y,
                                                     Controls::OffsetRangeSwitch + row);
        addParam(offsetRangePickList);
        popupMenus.push_back(offsetRangePickList->menu());

        auto *multiplierRangePickList = picklist::button(slug, "multiplier-range", multiplierRanges.size(), module,
                                                         column4, y, Controls::MultiplierRangeSwitch + row);
        addParam(multiplierRangePickList);
        popupMenus.push_back(multiplierRangePickList->menu());

        auto const updateRangePickList = [offsetRangePickList, multiplierRangePickList](bool isMultiply) {
          if (isMultiply) {
            offsetRangePickList->hide();
            multiplierRangePickList->show();
          } else {
            offsetRangePickList->show();
            multiplierRangePickList->hide();
          }
        };

        addParam(new OperatorSwitch{updateRangePickList, slug, module, column2, y, Controls::OperationSwitch + row});
      }

      for (auto *popupMenu : popupMenus) {
        addChild(popupMenu);
      }
    }
  }; // namespace func
} // namespace func
} // namespace dhe
