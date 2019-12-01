#pragma once

#include "PanelAssets.h"

#include <componentlibrary.hpp>
#include <string>

namespace dhe {

template <typename P> class Knob : public rack::componentlibrary::RoundKnob {
public:
  explicit Knob(std::string const &size) {
    static const auto prefix = std::string{"knob-"};
    setSvg(controlSvg<P>(prefix + size));
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
} // namespace dhe
