#pragma once

#include "assets.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dhe {
template <typename TPanel, typename TStyle>
struct ButtonWidget : public rack::app::SvgSwitch {
  using Action = std::function<void(bool)>;

  ButtonWidget() {
    auto const prefix = std::string{TPanel::svg_dir} + "/" + TStyle::slug + "-";
    addFrame(load_svg(prefix + "released"));
    addFrame(load_svg(prefix + "pressed"));
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgSwitch::onChange(e);
    auto const value = static_cast<bool>(this->getParamQuantity()->getValue());
    action_(value);
  }

  void on_change(Action const &action) { action_ = action; }

private:
  Action action_ = [](bool) {};
};

} // namespace dhe
