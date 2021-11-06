#pragma once

#include "rack.hpp"

#include <functional>

namespace dhe {
template <typename TPanel, typename TStyle, typename TValue = int>
struct SwitchWidget : public rack::app::SvgSwitch {
  using TAction = std::function<void(TValue)>;

  SwitchWidget() {
    auto const panel_prefix = std::string{TPanel::svg_dir} + "/";
    for (auto const &frame_name : TStyle::frame_names()) {
      addFrame(load_svg(panel_prefix + frame_name));
    }
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgSwitch::onChange(e);
    auto const value =
        static_cast<TValue>(this->getParamQuantity()->getValue());
    action_(value);
  }

  void on_change(TAction const &action) { action_ = action; }

private:
  TAction action_ = [](TValue) {};
};

} // namespace dhe
