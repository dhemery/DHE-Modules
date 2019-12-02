#pragma once

#include "BlossomControls.h"
#include "config/CommonConfig.h"

#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace blossom {
  static auto constexpr ratioRange = Range{1.F, 17.F};

  class RatioKnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override {
      auto const rotation = getValue();
      auto const unquantizedRatio = ratioRange.scale(rotation);
      auto const isQuantized = positionOf(module->params[BlossomControls::FreeRatioSwitch]) == 0;
      return isQuantized ? std::round(unquantizedRatio) : unquantizedRatio;
    }

    void setDisplayValue(float bounceRatio) override {
      auto const rotation = ratioRange.normalize(bounceRatio);
      setValue(rotation);
    }
  };

  static inline void configRatioKnob(rack::engine::Module *module, int knobId) {
    module->configParam<RatioKnobParamQuantity>(knobId, 0.F, 1.F, centeredRotation, "Ratio", "x");
  }

} // namespace blossom
} // namespace dhe