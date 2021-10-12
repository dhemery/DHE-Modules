#pragma once

#include "./blossom-controls.h"
#include "config/common-config.h"

#include "rack.hpp"

namespace dhe {
namespace blossom {
static auto constexpr ratio_range = Range{1.F, 17.F};

class RatioKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    auto const unquantized_ratio = ratio_range.scale(rotation);
    auto const is_quantized =
        position_of(module->params[BlossomControls::FreeRatioSwitch]) == 0;
    return is_quantized ? std::round(unquantized_ratio) : unquantized_ratio;
  }

  void setDisplayValue(float bounce_ratio) override {
    auto const rotation = ratio_range.normalize(bounce_ratio);
    setValue(rotation);
  }
};

static inline void config_ratio_knob(rack::engine::Module *module,
                                     int knob_id) {
  module->configParam<RatioKnobParamQuantity>(knob_id, 0.F, 1.F,
                                              centered_rotation, "Ratio", "x");
}

} // namespace blossom
} // namespace dhe