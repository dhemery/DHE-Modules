#pragma once

#include "func-controls.h"
#include "widgets/control-widgets.h"

#include <functional>
#include <utility>
#include <widget/event.hpp>

namespace dhe {
namespace func {

struct OffsetRangeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    auto names = std::vector<std::string>{};
    auto constexpr prefix = "offset-range-";
    for (size_t position = 1; position <= offset_ranges.size(); position++) {
      names.push_back(prefix + std::to_string(position));
    }
    return names;
  }
};

struct MultiplierRangeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    auto names = std::vector<std::string>{};
    auto constexpr prefix = "multiplier-range-";
    for (size_t position = 1; position <= offset_ranges.size(); position++) {
      names.push_back(prefix + std::to_string(position));
    }
    return names;
  }
};

template <typename PanelT>
class OperatorSwitch : public SwitchWidget<PanelT, ThumbSwitch<2>> {
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
    SwitchWidget<PanelT, ThumbSwitch<2>>::onChange(e);
    auto const is_multiply = this->getParamQuantity()->getValue() > 0.5;
    operator_changed_to_(is_multiply);
  }

private:
  std::function<void(bool)> operator_changed_to_ = [](bool /*unused*/) {};
};
} // namespace func

} // namespace dhe
