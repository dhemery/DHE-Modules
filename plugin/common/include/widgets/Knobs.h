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

  template <typename Panel>
  static auto install(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> LargeKnob * {
    return installParam<LargeKnob>(panel, module, x, y, index);
  }
};

template <typename P> class MediumKnob : public Knob<P> {
public:
  MediumKnob() : Knob<P>("medium") {}

  template <typename Panel>
  static auto install(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> MediumKnob * {
    return installParam<MediumKnob>(panel, module, x, y, index);
  }
};

template <typename P> class SmallKnob : public Knob<P> {
public:
  SmallKnob() : Knob<P>("small") {}

  template <typename Panel>
  static auto install(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> SmallKnob * {
    return installParam<SmallKnob>(panel, module, x, y, index);
  }
};

template <typename P> class TinyKnob : public Knob<P> {
public:
  TinyKnob() : Knob<P>("tiny") {}

  template <typename Panel>
  static auto install(Panel *panel, rack::engine::Module *module, float x, float y, int index) -> TinyKnob * {
    return installParam<TinyKnob>(panel, module, x, y, index);
  }
};
} // namespace dhe
