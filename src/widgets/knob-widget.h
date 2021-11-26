#pragma once

#include "assets.h"
#include "components/cxmath.h"
#include "rack.hpp"

namespace dhe {
template <typename T> struct KnobWidget : public rack::app::SvgKnob {
  using Value = typename T::ValueType;
  using Action = std::function<void(Value)>;

  KnobWidget() {
    auto knob_svg = load_svg(T::svg_dir, T::svg_file);
    setSvg(knob_svg);
    minAngle = -0.83F * pi;
    maxAngle = 0.83F * M_PI;
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgKnob::onChange(e);
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
    action_(static_cast<Value>(this->getParamQuantity()->getValue()));
  }
  Action action_ = [](Value) {};
};

} // namespace dhe
