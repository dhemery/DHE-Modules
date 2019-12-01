#pragma once

#include "CommonWidgets.h"
#include "PanelAssets.h"

#include <componentlibrary.hpp>
#include <string>

namespace dhe {

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
} // namespace dhe
