#pragma once

#include "assets.h"

#include "rack.hpp"

#include <functional>
#include <string>

namespace dhe {
template <typename T> struct ButtonWidget : public rack::app::SvgSwitch {
  using Action = std::function<void(bool)>;

  ButtonWidget() {
    auto const prefix = std::string{T::svg_dir} + "/" + T::slug + "-";
    addFrame(load_svg(prefix + "released"));
    addFrame(load_svg(prefix + "pressed"));
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgSwitch::onChange(e);
    notify();
  }

  void on_change(Action const &action) {
    action_ = action;
    if (module != nullptr) {
      notify();
    }
  }

private:
  void notify() {
    action_(static_cast<bool>(this->getParamQuantity()->getValue()));
  }
  Action action_ = [](bool) {};
};

} // namespace dhe
