#pragma once

#include <app.hpp>
#include <componentlibrary.hpp>

namespace DHE {
template <typename P> class Knob : public rack::RoundKnob {
public:
  explicit Knob(const std::string &size) {
    static const auto prefix = std::string{"knob-"};
    setSVG(P::svg(prefix + size));
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

template <typename P>
class Button : public rack::SVGSwitch, public rack::MomentarySwitch {
public:
  explicit Button(const std::string &name = "button") {
    addFrame(P::svg(name + "-1"));
    addFrame(P::svg(name + "-2"));
  }
};

template <typename P> class ReverseButton : public Button<P> {
public:
  ReverseButton() : Button<P>{"button-reverse"} {}
};

template <typename P, int N>
class Toggle : public rack::SVGSwitch, public rack::ToggleSwitch {
public:
  explicit Toggle(const std::string &name = "thumb-" + std::to_string(N)) {
    auto base = name + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(P::svg(base + std::to_string(position)));
    }
  }

  auto position() const -> int { return static_cast<int>(this->value); }

  static constexpr auto size = N;
};
} // namespace DHE
