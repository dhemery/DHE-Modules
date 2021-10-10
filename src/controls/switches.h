#pragma once

#include "widgets/dimensions.h"
#include "widgets/panel-assets.h"

#include <app/SvgSwitch.hpp>
#include <helpers.hpp>

#include <string>
#include <vector>

namespace dhe {

template <typename PanelT, typename FrameT, typename ValueT>
struct SwitchWidget : public rack::app::SvgSwitch {
  using ActionT = std::function<void(ValueT)>;

  SwitchWidget() {
    auto const panel_prefix = std::string{PanelT::svg_dir} + "/";
    for (auto const &frame_name : FrameT::frame_names()) {
      addFrame(load_svg(panel_prefix + frame_name));
    }
    shadow->opacity = 0.F;
  }

  void set_action(ActionT const &action) { action_ = action; }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgSwitch::onChange(e);
    auto const value =
        static_cast<ValueT>(this->getParamQuantity()->getValue());
    action_(value);
  }

private:
  ActionT action_ = [](ValueT /*selection*/) {};
};

static inline auto stepper_frame_names(std::string const &frame_prefix,
                                       size_t n) -> std::vector<std::string> {
  auto frame_names = std::vector<std::string>{};
  auto const prefix = frame_prefix + "-";
  for (size_t position = 1; position <= n; position++) {
    frame_names.push_back(prefix + std::to_string(position));
  }
  return frame_names;
}

template <int N> struct ThumbSwitch {
  static inline auto frame_names() -> std::vector<std::string> {
    static auto const frame_names =
        stepper_frame_names("toggle-" + std::to_string(N), N);
    return frame_names;
  }

  template <typename PanelT>
  static inline void install(PanelT *panel, int id, float xmm, float ymm) {
    panel->addParam(
        rack::createParamCentered<SwitchWidget<PanelT, ThumbSwitch, int>>(
            mm2px(xmm, ymm), panel->getModule(), id));
  }
};
} // namespace dhe
