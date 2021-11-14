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
  template <typename P, typename S, typename V> struct Config {
    using value_type = typename V::value_type; // NOLINT
    static auto constexpr size = V::size;
    static auto constexpr svg_dir = P::svg_dir;
    static auto constexpr slug = S::slug;
  };

  template <typename V, typename S, typename P>
  static inline auto install(P *panel, int id, float xmm, float ymm)
      -> SwitchWidget<Config<P, S, V>> * {
    auto *widget = rack::createParamCentered<SwitchWidget<Config<P, S, V>>>(
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
  template <int N> struct Size {};

  template <typename V> struct Style {
    static auto constexpr slug = Size<V::size>::slug;
  };

  template <typename V, typename P>
  static inline auto install(P *panel, int id, float xmm, float ymm)
      -> SwitchWidget<Switch::Config<P, Style<V>, V>> * {
    return Switch::install<V, Style<V>>(panel, id, xmm, ymm);
  }
};

template <> struct ThumbSwitch::Size<2> {
  static auto constexpr slug = "toggle-2";
};

template <> struct ThumbSwitch::Size<3> {
  static auto constexpr slug = "toggle-3";
};

struct Stepper {
  template <typename V> struct Style {
    static auto constexpr slug = V::stepper_slug;
  };

  template <typename V, typename P>
  static inline auto install(P *panel, int id, float xmm, float ymm)
      -> SwitchWidget<Switch::Config<P, Style<V>, V>> * {
    return Switch::install<V, Style<V>>(panel, id, xmm, ymm);
  }
};

} // namespace dhe
