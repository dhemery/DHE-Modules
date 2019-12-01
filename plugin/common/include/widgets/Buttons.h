#pragma once

#include "CommonWidgets.h"
#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <string>

namespace dhe {

class Button : public rack::app::SvgSwitch {
public:
  void setGraphics(std::string const &moduleSlug, float x, float y) {
    this->addFrame(controlSvg(moduleSlug, buttonName + "-1"));
    this->addFrame(controlSvg(moduleSlug, buttonName + "-2"));
    positionCentered(this, x, y);
  }

protected:
  Button(std::string name, bool momentary) : buttonName{std::move(name)} { this->momentary = momentary; }

private:
  std::string const buttonName;
}; // namespace DHE

class MomentaryButton : public Button {
public:
  explicit MomentaryButton(std::string const &name = "button") : Button{name, true} {}
}; // namespace DHE

class ReverseMomentaryButton : public MomentaryButton {
public:
  ReverseMomentaryButton(std::string const &name = "button-reversed") : MomentaryButton(name) {}
};

class ToggleButton : public Button {
public:
  explicit ToggleButton(std::string const &name = "button") : Button{name, false} {}
}; // namespace DHE

} // namespace dhe
