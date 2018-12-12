#pragma once

#include <app.hpp>
#include <componentlibrary.hpp>

namespace DHE {
template <typename T> class Knob : public rack::RoundKnob {
public:
  explicit Knob(const std::string &size) {
    static const auto prefix = std::string{"knob-"};
    setSVG(T::svg(prefix + size));
    shadow->opacity = 0.f;
  }
};

template <typename T> class LargeKnob : public Knob<T> {
public:
  LargeKnob() : Knob<T>("large") {}
};

template <typename T> class MediumKnob : public Knob<T> {
public:
  MediumKnob() : Knob<T>("medium") {}
};

template <typename T> class SmallKnob : public Knob<T> {
public:
  SmallKnob() : Knob<T>("small") {}
};

template <typename T> class TinyKnob : public Knob<T> {
public:
  TinyKnob() : Knob<T>("tiny") {}
};

template <typename T>
class Button : public rack::SVGSwitch, public rack::MomentarySwitch {
public:
  explicit Button(const std::string &type = "normal") {
    static const auto base = std::string{"button-"};
    const auto prefix = base + type + "-";
    addFrame(T::svg(prefix + "1"));
    addFrame(T::svg(prefix + "2"));
  }
};

template <typename T> class ReverseButton : public Button<T> {
public:
  ReverseButton() : Button<T>{"reverse"} {}
};

template <typename T, int N>
class Switch : public rack::SVGSwitch, public rack::ToggleSwitch {
public:
  explicit Switch(const std::string& name) {
    auto base = name + "-" ;
    for (int position = 1; position <= size; position++) {
      addFrame(T::svg(base + std::to_string(position)));
    }
  }

  auto position() const -> int {
    return static_cast<int>(this->value);
  }

  static constexpr auto size = N;
};

template <typename T, int N>
class ThumbSwitch : public Switch<T,N> {
public:
  explicit ThumbSwitch(const std::string& name = "thumb-" + std::to_string(N)) : Switch<T,N>{name} {}
};
} // namespace DHE
