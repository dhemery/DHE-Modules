#pragma once

#include "XycloidControls.h"
#include "components/Sigmoid.h"
#include "config/CommonConfig.h"

#include <array>

namespace dhe {
namespace xycloid {
  using Controls = XycloidControls;

  static auto constexpr wobbleDepthRange = Range{0.F, 1.F};
  static auto constexpr phaseOffsetRange = Range{-180.F, 180.F};
  static auto constexpr maxWobbleRatio = 16.F;
  static auto constexpr inwardWobbleRatioRange = Range{0.F, -maxWobbleRatio};
  static auto constexpr outwardWobbleRatioRange = Range{0.F, maxWobbleRatio};
  static auto constexpr bidirectionalWobbleRatioRange = Range{-maxWobbleRatio, maxWobbleRatio};

  static auto const wobbleRatioRanges
      = std::array<Range const *, 3>{&inwardWobbleRatioRange, &bidirectionalWobbleRatioRange, &outwardWobbleRatioRange};

  static inline auto wobbleRatioRange(rack::engine::Module const *xycloid) -> Range const * {
    return selectedRange<3>(xycloid->params[Controls::DirectionSwitch], wobbleRatioRanges);
  }

  static inline auto wobbleRatio(rack::engine::Module const *module, float rotation) -> float {
    auto const isQuantized = positionOf(module->params[Controls::FreeRatioSwitch]) == 0;
    auto const unquantizedRatio = wobbleRatioRange(module)->scale(rotation);
    return isQuantized ? std::round(unquantizedRatio) : unquantizedRatio;
  }

  class RatioKnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override { return wobbleRatio(module, getValue()); }

    void setDisplayValue(float bounceRatio) override {
      auto const range = wobbleRatioRange(module);
      auto const rotation = range->normalize(bounceRatio);
      setValue(rotation);
    }
  };

  void configWobbleRatioKnob(rack::engine::Module *module, int knobId) {
    module->configParam<RatioKnobParamQuantity>(knobId, 0.F, 1.F, centeredRotation, "Ratio", "x");
  }

} // namespace xycloid
} // namespace dhe