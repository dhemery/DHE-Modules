#pragma once

#include "assets.h"

#include "rack.hpp"

namespace dhe {
template <typename P, typename S, typename V>
struct KnobWidget : public rack::app::SvgKnob {
  using Action = std::function<void(V)>;

  KnobWidget() {
    auto knob_svg = load_svg(P::svg_dir, S::svg_file);
    setSvg(knob_svg);
    minAngle = -0.83F * pi;
    maxAngle = 0.83F * M_PI;
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgKnob::onChange(e);
    auto const value = static_cast<V>(this->getParamQuantity()->getValue());
    action_(value);
  }

  void on_change(Action const &action) { action_ = action; }

private:
  Action action_ = [](V) {};
};

} // namespace dhe
