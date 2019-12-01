#pragma once

#include "PanelAssets.h"

#include <componentlibrary.hpp>
#include <string>
#include <utility>

namespace dhe {

template <typename P> class Knob : public rack::componentlibrary::RoundKnob {
public:
  explicit Knob(std::string sizeName) : sizeName{std::move(sizeName)} {}

  void setGraphics(P *panel) { setSvg(controlSvg<P>(controlName(sizeName))); }

private:
  static auto controlName(std::string const &sizeName) -> std::string {
    static const auto controlNamePrefix = std::string{"knob-"};
    return controlNamePrefix + sizeName;
  }

  std::string const sizeName;
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
} // namespace dhe
