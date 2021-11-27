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
    using ValueType = typename V::ValueType;
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
                            typename T::ValueType value)
      -> SwitchQuantity<typename T::ValueType> * {
    static auto const labels =
        std::vector<std::string>{T::labels.cbegin(), T::labels.cend()};
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(value);
    return module->configSwitch<SwitchQuantity<typename T::ValueType>>(
        id, 0.F, max, default_value, name, labels);
  }
};

struct ThumbSwitch {
  template <typename Panel, int Size> struct Widget : rack::app::SvgSwitch {
    Widget() {
      auto const prefix =
          std::string{Panel::svg_dir} + "/toggle-" + std::to_string(Size) + '-';
      for (auto position = 1; position <= Size; position++) {
        addFrame(load_svg(prefix + std::to_string(position)));
      }
      shadow->opacity = 0.F;
    }
  };

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
