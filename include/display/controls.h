#pragma once
#include <utility>

#include "app/SvgSwitch.hpp"
#include "componentlibrary.hpp"

#include "display/controls.h"
#include "display/menus.h"

namespace DHE {

template <typename P> class Knob : public rack::componentlibrary::RoundKnob {
public:
  explicit Knob(const std::string &size) {
    static const auto prefix = std::string{"knob-"};
    setSvg(P::svg(prefix + size));
    shadow->opacity = 0.f;
  }
};

template <typename P> class LargeKnob : public Knob<P> {
public:
  LargeKnob() : Knob<P>("large") {}
};

template <typename P> class MediumKnob : public Knob<P> {
public:
  MediumKnob() : Knob<P>("medium") {}
};

template <typename P> class SmallKnob : public Knob<P> {
public:
  SmallKnob() : Knob<P>("small") {}
};

template <typename P> class TinyKnob : public Knob<P> {
public:
  TinyKnob() : Knob<P>("tiny") {}
};

template <typename P> class Button : public rack::app::SvgSwitch {
public:
  explicit Button(const std::string &name = "button") {
    momentary = true;
    addFrame(P::svg(name + "-1"));
    addFrame(P::svg(name + "-2"));
  }
}; // namespace DHE

template <typename P> class ReverseButton : public Button<P> {
public:
  ReverseButton() : Button<P>("button-reversed") {}
};

template <typename P, int N> class Toggle : public rack::app::SvgSwitch {
public:
  explicit Toggle(const std::string &name = "toggle-" + std::to_string(N)) {
    auto base = name + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(P::svg(base + std::to_string(position)));
    }
  }

  static constexpr auto size = N;
};
} // namespace DHE
