#pragma once

#include "blossom-controls.h"

#include "params/common-config.h"

#include "rack.hpp"

namespace dhe {
namespace blossom {
static auto constexpr ratio_range = Range{1.F, 17.F};

class RatioKnob : public rack::engine::ParamQuantity {
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

  static inline void config(rack::engine::Module *module, int id) {
    module->configParam<RatioKnob>(id, 0.F, 1.F, centered_rotation, "Ratio",
                                   "x");
  }
};

} // namespace blossom
} // namespace dhe
