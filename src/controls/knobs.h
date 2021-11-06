#pragma once

#include "components/range.h"
#include "knob-quantity.h"
#include "knob-widget.h"
#include "panels/dimensions.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {

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

  template <typename TQuantity, typename TValue>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, TValue value)
      -> KnobQuantity<TValue> * {
    static auto const range = TQuantity::range();
    return module->configParam<KnobQuantity<TValue>>(
        id, range.lower_bound(), range.upper_bound(), value, name,
        TQuantity::unit, 0.F, TQuantity::display_multiplier,
        TQuantity::display_offset);
  }
};

} // namespace dhe
