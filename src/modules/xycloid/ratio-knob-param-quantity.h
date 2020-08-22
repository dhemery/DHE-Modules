#pragma once

#include "components/sigmoid.h"
#include "config/common-config.h"
#include "xycloid-controls.h"

#include <array>

namespace dhe {
namespace xycloid {
using Controls = XycloidControls;

static auto constexpr max_ratio = 16.F;
static auto constexpr inward_ratio_range = Range{0.F, -max_ratio};
static auto constexpr outward_ratio_range = Range{0.F, max_ratio};
static auto constexpr bidirectional_ratio_range = Range{-max_ratio, max_ratio};

static auto constexpr ratio_ranges = std::array<Range const *, 3>{
    &inward_ratio_range, &bidirectional_ratio_range, &outward_ratio_range};

static inline auto ratio_range(rack::engine::Module const *module)
    -> Range const * {
  return selected_range<3>(module->params[Controls::DirectionSwitch],
                           ratio_ranges);
}

static inline auto ratio(rack::engine::Module const *module, float rotation)
    -> float {
  auto const is_quantized =
      position_of(module->params[Controls::FreeRatioSwitch]) == 0;
  auto const unquantized_ratio = ratio_range(module)->scale(rotation);
  return is_quantized ? std::round(unquantized_ratio) : unquantized_ratio;
}

class RatioKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override { return ratio(module, getValue()); }

  void setDisplayValue(float bounce_ratio) override {
    const auto *const range = ratio_range(module);
    auto const rotation = range->normalize(bounce_ratio);
    setValue(rotation);
  }
};

static inline void config_ratio_knob(rack::engine::Module *module,
                                     int knob_id) {
  module->configParam<RatioKnobParamQuantity>(knob_id, 0.F, 1.F,
                                              centered_rotation, "Ratio", "x");
}

} // namespace xycloid
} // namespace dhe
