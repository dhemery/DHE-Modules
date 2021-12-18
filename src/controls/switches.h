#pragma once

#include "panels/assets.h"
#include "panels/dimensions.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {

struct Switch {
  using Quantity = rack::engine::SwitchQuantity;

  struct Widget : rack::app::SvgSwitch {
    Widget() { shadow->opacity = 0.F; }
  };

  template <typename Style>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            typename Style::ValueType value) -> Quantity * {
    static auto const labels =
        std::vector<std::string>{Style::labels.cbegin(), Style::labels.cend()};
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(value);
    return module->configSwitch<Quantity>(id, 0.F, max, default_value, name,
                                          labels);
  }
};

struct ThumbSwitch {
  template <typename Panel, int Size> struct Widget : Switch::Widget {
    Widget() {
      auto const prefix = std::string{Panel::svg_dir} + "/thumb-switch-" +
                          std::to_string(Size) + '-';
      for (auto position = 1; position <= Size; position++) {
        addFrame(load_svg(prefix + std::to_string(position)));
      }
    }
  };
};

struct Stepper {
  template <typename Panel, typename Style> struct Widget : Switch::Widget {
    Widget() {
      auto const prefix = std::string{Panel::svg_dir} + "/" + Style::slug + '-';
      for (auto position = 1UL; position <= Style::size; position++) {
        addFrame(load_svg(prefix + std::to_string(position)));
      }
    }
  };

  template <typename Style, typename Panel>
  static inline auto install(Panel *panel, int param_id, float xmm, float ymm)
      -> Widget<Panel, Style> * {
    auto *w = rack::createParamCentered<Widget<Panel, Style>>(
        mm2px(xmm, ymm), panel->getModule(), param_id);
    panel->addParam(w);
    return w;
  }
};

} // namespace dhe
