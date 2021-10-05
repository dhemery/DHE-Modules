#pragma once

#include "func-controls.h"
#include "widgets/control-widgets.h"

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <widget/event.hpp>

namespace dhe {
namespace func {

struct OffsetRangeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("offset-range", offset_ranges.size());
    return frame_names;
  }
};

struct MultiplierRangeStepper {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("multiplier-range", multiplier_ranges.size());
    return frame_names;
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
