#pragma once

#include "params/switch-quantity.h"
#include "widgets/dimensions.h"
#include "widgets/switch-widget.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dhe {

struct Switch {
  template <typename V, typename S, typename P>
  static inline auto install(P *panel, int id, float xmm, float ymm)
      -> SwitchWidget<P, S, V> * {
    auto *widget = rack::createParamCentered<SwitchWidget<P, S, V>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename T>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            typename T::value_type value)
      -> SwitchQuantity<typename T::value_type> * {
    auto const labels = T::labels();
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(value);
    return module->configSwitch<SwitchQuantity<typename T::value_type>>(
        id, 0.F, max, default_value, name, labels);
  }
};

struct ThumbSwitch {
  template <typename V> struct Style {
    static auto slug() -> std::string const & {
      static auto const size = V::labels().size();
      static auto const slug = "toggle-" + std::to_string(size);
      return slug;
    }
  };

  template <typename V, typename P>
  static inline auto install(P *panel, int id, float xmm, float ymm)
      -> SwitchWidget<P, Style<V>, V> * {
    return Switch::install<V, Style<V>>(panel, id, xmm, ymm);
  }
};

struct Stepper {
  template <typename V> struct Style {
    static inline auto slug() -> std::string const & {
      static auto const slug = std::string{V::stepper_slug};
      return slug;
    }
  };

  template <typename V, typename P>
  static inline auto install(P *panel, int id, float xmm, float ymm)
      -> SwitchWidget<P, Style<V>, V> * {
    return Switch::install<V, Style<V>>(panel, id, xmm, ymm);
  }
};

} // namespace dhe
