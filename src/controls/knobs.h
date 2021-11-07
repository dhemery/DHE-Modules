#pragma once

#include "params/knob-quantity.h"
#include "params/scaled-knob-quantity.h"
#include "widgets/dimensions.h"
#include "widgets/knob-widget.h"

#include "rack.hpp"

#include <string>

namespace dhe {

struct Large {
  static auto constexpr svg_file = "knob-large";
};

struct Medium {
  static auto constexpr svg_file = "knob-medium";
};

struct Small {
  static auto constexpr svg_file = "knob-small";
};

struct Tiny {
  static auto constexpr svg_file = "knob-tiny";
};

struct Knob {
  template <typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, float> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle, float>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename TQuantity>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float value)
      -> ScaledKnobQuantity * {
    static auto const range = TQuantity::range();
    return module->configParam<ScaledKnobQuantity>(
        id, range.lower_bound(), range.upper_bound(), value, name,
        TQuantity::unit, 0.F, TQuantity::display_multiplier,
        TQuantity::display_offset);
  }
};

struct IntKnob {
  template <typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, int> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle, int>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    widget->snap = true;
    return widget;
  }

  template <typename TQuantity>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, int value)
      -> KnobQuantity<int> * {
    auto *pq = module->configParam<KnobQuantity<int>>(
        id, TQuantity::min_value, TQuantity::max_value, value, name,
        TQuantity::unit, 0.F, 1.F, TQuantity::display_value_offset);
    pq->snapEnabled = true;
    return pq;
  }
};

} // namespace dhe
