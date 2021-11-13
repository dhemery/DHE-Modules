#pragma once

#include "assets.h"

#include "rack.hpp"

namespace dhe {
template <typename TPanel, typename TStyle, typename TValue>
struct KnobWidget : public rack::app::SvgKnob {
  using TAction = std::function<void(TValue)>;

  KnobWidget() {
    auto knob_svg = load_svg(TPanel::svg_dir, TStyle::svg_file);
    setSvg(knob_svg);
    minAngle = -0.83F * pi;
    maxAngle = 0.83F * M_PI;
    shadow->opacity = 0.F;
  }

  void onChange(const rack::event::Change &e) override {
    rack::app::SvgKnob::onChange(e);
    auto const value =
        static_cast<TValue>(this->getParamQuantity()->getValue());
    action_(value);
  }

  void on_change(TAction const &action) { action_ = action; }

private:
  TAction action_ = [](TValue) {};
};

} // namespace dhe
