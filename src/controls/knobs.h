#pragma once

#include "knob-widget.h"
#include "panels/dimensions.h"
#include "scaled-quantity.h"

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
  template <typename TStyle, typename TValue = float, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, TValue> * {
    auto *widget =
        rack::createParamCentered<KnobWidget<TPanel, TStyle, TValue>>(
            mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename TQuantity>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float value)
      -> ScaledQuantity * {
    static auto const range = TQuantity::range();
    return module->configParam<ScaledQuantity>(
        id, range.lower_bound(), range.upper_bound(), value, name,
        TQuantity::unit, 0.F, TQuantity::display_multiplier,
        TQuantity::display_offset);
  }
};

} // namespace dhe
