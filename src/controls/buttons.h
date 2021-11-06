#pragma once

#include "params/discrete-quantity.h"
#include "widgets/dimensions.h"
#include "widgets/switch-widget.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {

struct Normal {
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const names =
        std::vector<std::string>{"button-released", "button-pressed"};
    return names;
  }
};

struct Reversed {
  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const names = std::vector<std::string>{"output-button-released",
                                                       "output-button-pressed"};
    return names;
  }
};

struct Momentary {
  static auto constexpr momentary = true;
};

struct Toggle {
  static auto constexpr momentary = false;
};

struct Button {
  template <typename TBehavior, typename TStyle = Normal, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> SwitchWidget<TPanel, TStyle, bool> * {
    auto widget = rack::createParamCentered<SwitchWidget<TPanel, TStyle, bool>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    widget->momentary = TBehavior::momentary;
    panel->addParam(widget);
    return widget;
  }

  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, bool pressed = false)
      -> DiscreteQuantity<bool> * {
    auto const default_value = static_cast<float>(pressed);
    auto *pq = module->configSwitch<DiscreteQuantity<bool>>(
        id, 0.F, 1.F, default_value, name);
    return pq;
  }
};

} // namespace dhe
