#pragma once

#include "CommonWidgets.h"
#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <string>

namespace dhe {

template <typename P> class Button : public rack::app::SvgSwitch {
public:
  explicit Button(std::string const &name = "button") {
    momentary = true;
    addFrame(controlSvg<P>(name + "-1"));
    addFrame(controlSvg<P>(name + "-2"));
  }
}; // namespace DHE

template <typename P> class ReverseButton : public Button<P> {
public:
  ReverseButton() : Button<P>("button-reversed") {}
};

template <typename P> class ToggleButton : public rack::app::SvgSwitch {
public:
  explicit ToggleButton(std::string const &name = "button") {
    addFrame(controlSvg<P>(name + "-1"));
    addFrame(controlSvg<P>(name + "-2"));
  }
}; // namespace DHE

} // namespace dhe
