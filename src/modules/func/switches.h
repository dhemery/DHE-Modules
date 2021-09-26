#pragma once

#include "func-controls.h"
#include "widgets/control-widgets.h"

#include <functional>
#include <utility>
#include <widget/event.hpp>

namespace dhe {
namespace func {

template <typename PanelT>
class OffsetRangeStepper : public Toggle<PanelT, offset_ranges.size()> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> OffsetRangeStepper<PanelT> * {
    return rack::createParamCentered<OffsetRangeStepper>(mm2px(xmm, ymm),
                                                         module, index);
  }

  OffsetRangeStepper() : Toggle<PanelT, offset_ranges.size()>{"offset-range"} {}
};

template <typename PanelT>
class MultiplierRangeStepper : public Toggle<PanelT, multiplier_ranges.size()> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> MultiplierRangeStepper * {
    return rack::createParamCentered<MultiplierRangeStepper>(mm2px(xmm, ymm),
                                                             module, index);
  }

  MultiplierRangeStepper()
      : Toggle<PanelT, multiplier_ranges.size()>{"multiplier-range"} {}
};

template <typename PanelT> class OperatorSwitch : public Toggle<PanelT, 2> {
public:
  static inline auto create(rack::engine::Module *module, float xmm, float ymm,
                            int index, std::function<void(bool)> const &action)
      -> OperatorSwitch * {
    auto toggle = rack::createParamCentered<OperatorSwitch>(mm2px(xmm, ymm),
                                                            module, index);
    toggle->operator_changed_to_ = action;
    return toggle;
  }

  OperatorSwitch() = default;

  void onChange(const rack::event::Change &e) override {
    Toggle<PanelT, 2>::onChange(e);
    auto const is_multiply = this->getParamQuantity()->getValue() > 0.5;
    operator_changed_to_(is_multiply);
  }

private:
  std::function<void(bool)> operator_changed_to_ = [](bool /*unused*/) {};
};
} // namespace func

} // namespace dhe
