#pragma once

#include "panels/assets.h"
#include "panels/dimensions.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {

struct Switch {
  using Quantity = rack::engine::SwitchQuantity;

  template <typename T> struct Widget : public rack::app::SvgSwitch {
    Widget() {
      static auto const file_names = frame_file_names();
      auto const panel_prefix = std::string{T::svg_dir} + "/";
      for (auto const &file_name : file_names) {
        addFrame(load_svg(panel_prefix + file_name));
      }
      shadow->opacity = 0.F;
    }

  private:
    static inline auto frame_file_names() -> std::vector<std::string> {
      auto names = std::vector<std::string>{};
      auto const prefix = T::slug + std::string{"-"};
      auto const size = T::size;
      for (size_t position = 1; position <= size; position++) {
        names.push_back(prefix + std::to_string(position));
      }
      return names;
    }
  };

  template <typename P, typename S, typename V> struct Config {
    using ValueType = typename V::ValueType;
    static auto constexpr size = V::size;
    static auto constexpr svg_dir = P::svg_dir;
    static auto constexpr slug = S::slug;
  };

  template <typename V, typename S, typename P>
  static inline auto install(P *panel, int id, float xmm, float ymm)
      -> Widget<Config<P, S, V>> * {
    auto *widget = rack::createParamCentered<Widget<Config<P, S, V>>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename T>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            typename T::ValueType value) -> Quantity * {
    static auto const labels =
        std::vector<std::string>{T::labels.cbegin(), T::labels.cend()};
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(value);
    return module->configSwitch<Quantity>(id, 0.F, max, default_value, name,
                                          labels);
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
};

struct Stepper {
  template <typename Panel, typename Style>
  struct Widget : rack::app::SvgSwitch {
    Widget() {
      auto const prefix = std::string{Panel::svg_dir} + "/" + Style::slug + '-';
      for (auto position = 1UL; position <= Style::size; position++) {
        addFrame(load_svg(prefix + std::to_string(position)));
      }
      shadow->opacity = 0.F;
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
