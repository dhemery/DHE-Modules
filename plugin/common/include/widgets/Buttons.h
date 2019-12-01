#pragma once

#include "CommonWidgets.h"
#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <string>
#include <utility>

#include <iostream>

namespace dhe {

template <typename P> class Button : public rack::app::SvgSwitch {
public:
  void setGraphics(P *panel) {
    std::cerr << "Loading Button " << buttonName << " in panel " << P::moduleSlug << std::endl;
    this->addFrame(controlSvg<P>(buttonName + "-1"));
    this->addFrame(controlSvg<P>(buttonName + "-2"));
  }

protected:
  Button(std::string name, bool momentary) : buttonName{std::move(name)} { this->momentary = momentary; }

private:
  std::string const buttonName;
}; // namespace DHE

template <typename P> class MomentaryButton : public Button<P> {
public:
  explicit MomentaryButton(std::string const &name = "button") : Button<P>{name, true} {}
}; // namespace DHE

template <typename P> class ReverseMomentaryButton : public MomentaryButton<P> {
public:
  ReverseMomentaryButton(std::string const &name = "button-reversed") : MomentaryButton<P>(name) {}
};

template <typename P> class ToggleButton : public Button<P> {
public:
  explicit ToggleButton(std::string const &name = "button") : Button<P>{name, false} {}
}; // namespace DHE

} // namespace dhe
