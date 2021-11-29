#pragma once

#include "switches.h"

#include "panels/assets.h"
#include "panels/dimensions.h"

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
  using Quantity = Switch::Quantity;

  template <typename Panel, typename Style>
  struct Widget : public rack::app::SvgSwitch {
    Widget() {
      auto const prefix = std::string{Panel::svg_dir} + "/" + Style::slug + "-";
      addFrame(load_svg(prefix + "released"));
      addFrame(load_svg(prefix + "pressed"));
      shadow->opacity = 0.F;
    }
  };

  template <typename Behavior, typename Style = Normal, typename Panel>
  static inline auto install(Panel *panel, int id, float xmm, float ymm)
      -> Widget<Panel, Style> * {
    auto widget = rack::createParamCentered<Widget<Panel, Style>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    widget->momentary = Behavior::momentary;
    panel->addParam(widget);
    return widget;
  }

  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, bool pressed = false)
      -> Quantity * {
    auto const default_value = static_cast<float>(pressed);
    return module->configSwitch<Quantity>(id, 0.F, 1.F, default_value, name);
  }
};

} // namespace dhe
