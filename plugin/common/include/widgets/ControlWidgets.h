#pragma once

#include "CommonWidgets.h"
#include "PanelAssets.h"

#include <app/SvgSwitch.hpp>
#include <componentlibrary.hpp>
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

class Knob : public rack::componentlibrary::RoundKnob {
public:
  explicit Knob(std::string sizeName) : sizeName{std::move(sizeName)} {}

  void setGraphics(std::string const &moduleSlug, float x, float y) {
    setSvg(controlSvg(moduleSlug, controlName(sizeName)));
    positionCentered(this, x, y);
  }

private:
  static auto controlName(std::string const &sizeName) -> std::string {
    static const auto controlNamePrefix = std::string{"knob-"};
    return controlNamePrefix + sizeName;
  }

  std::string const sizeName;
};

class LargeKnob : public Knob {
public:
  LargeKnob() : Knob("large") {}
};

class MediumKnob : public Knob {
public:
  MediumKnob() : Knob("medium") {}
};

class SmallKnob : public Knob {
public:
  SmallKnob() : Knob("small") {}
};

class TinyKnob : public Knob {
public:
  TinyKnob() : Knob("tiny") {}
};

template <int N> class Toggle : public rack::app::SvgSwitch {
public:
  explicit Toggle(std::string name = "toggle-" + std::to_string(N)) : toggleName{std::move(name)} {}

  void setGraphics(std::string const &moduleSlug, float x, float y) {
    auto const controlNamePrefix = toggleName + "-";
    for (int position = 1; position <= size; position++) {
      addFrame(controlSvg(moduleSlug, controlNamePrefix + std::to_string(position)));
    }
    positionCentered(this, x, y);
  }

private:
  std::string const toggleName;
  static constexpr auto size = N;
};

} // namespace dhe
