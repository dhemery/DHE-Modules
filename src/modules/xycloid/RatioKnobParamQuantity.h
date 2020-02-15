#pragma once

#include "XycloidControls.h"
#include "components/Sigmoid.h"
#include "config/CommonConfig.h"

#include <array>

namespace dhe {
namespace xycloid {
  using Controls = XycloidControls;

  static auto constexpr maxRatio = 16.F;
  static auto constexpr inwardRatioRange = Range{0.F, -maxRatio};
  static auto constexpr outwardRatioRange = Range{0.F, maxRatio};
  static auto constexpr bidirectionalRatioRange = Range{-maxRatio, maxRatio};

  static auto const ratioRanges
      = std::array<Range const *, 3>{&inwardRatioRange, &bidirectionalRatioRange, &outwardRatioRange};

  static inline auto ratioRange(rack::engine::Module const *module) -> Range const * {
    return selectedRange<3>(module->params[Controls::DirectionSwitch], ratioRanges);
  }

  static inline auto ratio(rack::engine::Module const *module, float rotation) -> float {
    auto const isQuantized = positionOf(module->params[Controls::FreeRatioSwitch]) == 0;
    auto const unquantizedRatio = ratioRange(module)->scale(rotation);
    return isQuantized ? std::round(unquantizedRatio) : unquantizedRatio;
  }

  class RatioKnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override { return ratio(module, getValue()); }

    void setDisplayValue(float bounceRatio) override {
      auto const range = ratioRange(module);
      auto const rotation = range->normalize(bounceRatio);
      setValue(rotation);
    }
  };

  static inline void configRatioKnob(rack::engine::Module *module, int knobId) {
    module->configParam<RatioKnobParamQuantity>(knobId, 0.F, 1.F, centeredRotation, "Ratio", "x");
  }

} // namespace xycloid
} // namespace dhe
