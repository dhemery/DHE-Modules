#pragma once

#include "panels/dimensions.h"
#include "panels/panel-assets.h"

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
  template <typename TPanel, typename TFrame, typename TValue>
  struct Widget : public rack::app::SvgSwitch {
    using TAction = std::function<void(TValue)>;

    Widget() {
      auto const panel_prefix = std::string{TPanel::svg_dir} + "/";
      for (auto const &frame_name : TFrame::frame_names()) {
        addFrame(load_svg(panel_prefix + frame_name));
      }
      shadow->opacity = 0.F;
    }

    void set_action(TAction const &action) { action_ = action; }

    void onChange(const rack::event::Change &e) override {
      rack::app::SvgSwitch::onChange(e);
      auto const value =
          static_cast<TValue>(this->getParamQuantity()->getValue());
      action_(value);
    }

  private:
    TAction action_ = [](TValue) {};
  };

  static inline void config(rack::engine::Module *module, int id,
                            std::string const &name,
                            std::vector<std::string> const &labels,
                            int initial = 0) {
    auto const max = static_cast<float>(labels.size() - 1);
    auto const default_value = static_cast<float>(initial);
    module->configSwitch(id, 0.F, max, default_value, name, labels);
  }
};

template <int N> struct ThumbSwitch {
  using TAction = std::function<void(int)>;
  template <typename TPanel>
  using TWidget = Switch::Widget<TPanel, ThumbSwitch, int>;

  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const frame_names =
        numbered_frame_names("toggle-" + std::to_string(N), N);
    return frame_names;
  }

  template <typename TPanel>
  static inline auto install(
      TPanel *panel, int id, float xmm, float ymm,
      TAction const &action = [](int) {}) -> TWidget<TPanel> * {
    auto *widget = rack::createParamCentered<TWidget<TPanel>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    widget->set_action(action);
    panel->addParam(widget);
    return widget;
  }
};

} // namespace dhe
