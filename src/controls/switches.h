#pragma once

#include "params/picker-switch-quantity.h"
#include "params/switch-quantity.h"
#include "widgets/dimensions.h"
#include "widgets/switch-widget.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dhe {

static inline auto numbered_frame_names(std::string const &frame_prefix,
                                        size_t n) -> std::vector<std::string> {
  auto frame_names = std::vector<std::string>{};
  auto const prefix = frame_prefix + "-";
  for (size_t position = 1; position <= n; position++) {
    frame_names.push_back(prefix + std::to_string(position));
  }
  return frame_names;
}

struct Switch {
  template <typename TStyle, typename TValue = int, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, TStyle, TValue> * {
    auto *widget =
        rack::createParamCentered<SwitchWidget<TPanel, TStyle, TValue>>(
            mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename TValue>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            std::vector<std::string> const &labels,
                            TValue initial) -> SwitchQuantity<TValue> * {
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(initial);
    return module->configSwitch<SwitchQuantity<TValue>>(
        id, 0.F, max, default_value, name, labels);
  }
};

template <int N> struct ThumbSwitch {
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const frame_names =
        numbered_frame_names("toggle-" + std::to_string(N), N);
    return frame_names;
  }

  template <typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, ThumbSwitch<N>> * {
    return Switch::install<ThumbSwitch<N>>(panel, id, xmm, ymm);
  }
};

template <typename TStepper> struct Stepper {
  using Value = typename TStepper::TValue;
  using Quantity = SwitchQuantity<Value>;
  using Frame = Stepper<TStepper>;
  template <typename TPanel> using TWidget = SwitchWidget<TPanel, Frame, Value>;

  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const frame_names =
        numbered_frame_names(TStepper::frame_prefix, TStepper::labels().size());
    return frame_names;
  }

  template <typename TModule>
  static inline auto config(TModule *module, int id, std::string const &name,
                            Value value) -> Quantity * {
    return Switch::config(module, id, name, TStepper::labels(), value);
  }

  template <typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> TWidget<TPanel> * {
    return Switch::install<Frame, Value>(panel, id, xmm, ymm);
  }
};

struct Picker {
  template <typename TItems>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, int selection)
      -> PickerSwitchQuantity<TItems> * {
    auto const max_value = static_cast<float>(TItems::items().size() - 1);
    auto const default_value = static_cast<float>(selection);
    auto *pq = module->configSwitch<PickerSwitchQuantity<TItems>>(
        id, 0.F, max_value, default_value, name, TItems::labels());
    return pq;
  }
};

} // namespace dhe
