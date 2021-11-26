#pragma once

#include "params/switch-quantity.h"
#include "widgets/button-widget.h"
#include "widgets/dimensions.h"

#include "rack.hpp"

#include <string>

namespace dhe {

struct Normal {
  static auto constexpr slug = "button";
};

struct Reversed {
  static auto constexpr slug = "output-button";
};

struct Momentary {
  static auto constexpr momentary = true;
};

struct Toggle {
  static auto constexpr momentary = false;
};

struct Button {
  template <typename P, typename S> struct Config {
    static auto constexpr svg_dir = P::svg_dir;
    static auto constexpr slug = S::slug;
  };

  template <typename P, typename S> using Widget = ButtonWidget<Config<P, S>>;

  template <typename B, typename S = Normal, typename P>
  static inline auto install(P *panel, int id, float xmm, float ymm)
      -> Widget<P, S> * {
    auto widget = rack::createParamCentered<Widget<P, S>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    widget->momentary = B::momentary;
    panel->addParam(widget);
    return widget;
  }

  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, bool pressed = false)
      -> SwitchQuantity<bool> * {
    auto const default_value = static_cast<float>(pressed);
    return module->configSwitch<SwitchQuantity<bool>>(id, 0.F, 1.F,
                                                      default_value, name);
  }
};

} // namespace dhe
