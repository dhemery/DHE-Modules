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

struct Switch {
  template <typename TItems, typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, TStyle, TItems> * {
    auto *widget =
        rack::createParamCentered<SwitchWidget<TPanel, TStyle, TItems>>(
            mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename TItems>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            typename TItems::ValueType initial)
      -> SwitchQuantity<typename TItems::ValueType> * {
    auto const labels = TItems::labels();
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(initial);
    return module->configSwitch<SwitchQuantity<typename TItems::ValueType>>(
        id, 0.F, max, default_value, name, labels);
  }
};

struct ThumbSwitch {
  template <typename TItems> struct Style {
    static auto slug() -> std::string const & {
      static auto const size = TItems::labels().size();
      static auto const slug = "toggle-" + std::to_string(size);
      return slug;
    }
  };

  template <typename TItems, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, Style<TItems>, TItems> * {
    return Switch::install<TItems, Style<TItems>>(panel, id, xmm, ymm);
  }
};

struct Stepper {
  template <typename TItems> struct Style {
    static inline auto slug() -> std::string const & {
      static auto const slug = std::string{TItems::stepper_slug};
      return slug;
    }
  };

  template <typename TItems, typename TModule>
  static inline auto config(TModule *module, int id, std::string const &name,
                            typename TItems::ValueType value)
      -> SwitchQuantity<typename TItems::ValueType> * {
    return Switch::config<TItems>(module, id, name, value);
  }

  template <typename TItems, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, Style<TItems>, TItems> * {
    return Switch::install<TItems, Style<TItems>>(panel, id, xmm, ymm);
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
