#pragma once

#include <app/SvgSwitch.hpp>
#include <string>

namespace dhe {

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

template <typename P> class ToggleButton : public rack::app::SvgSwitch {
public:
  explicit ToggleButton(const std::string &name = "button") {
    addFrame(P::svg(name + "-1"));
    addFrame(P::svg(name + "-2"));
  }
}; // namespace DHE

} // namespace dhe