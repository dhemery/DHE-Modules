#pragma once

#include "xycloid-controls.h"

#include "components/range.h"
#include "components/sigmoid.h"

#include "rack.hpp"

#include <array>

namespace dhe {
namespace xycloid {

struct ThrobSpeed {
  static auto constexpr range = Range{-10.F, 10.F};
  static auto constexpr initial_speed_hz{1.F};
  static auto constexpr taper_curvature = -0.8F;
  static auto constexpr taper =
      sigmoid::s_taper_with_curvature(taper_curvature);

  static auto constexpr rotation_to_speed(float rotation) -> float {
    return range.scale(taper.apply(rotation));
  }

  static auto constexpr speed_to_rotation(float speed) -> float {
    return taper.invert(range.normalize(speed));
  }

  class Quantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return rotation_to_speed(getValue());
    }

    void setDisplayValue(float speed) override {
      setValue(speed_to_rotation(speed));
    }
  };

  static void config(rack::engine::Module *module, int knob_id) {
    static auto constexpr initial_rotation =
        speed_to_rotation(initial_speed_hz);

    module->configParam<Quantity>(knob_id, 0.F, 1.F, initial_rotation, "Speed",
                                  " Hz");
  }
};

struct WobbleRatio {
  using Controls = XycloidControls;

  static inline auto ratio_range(rack::engine::Module const *module) -> Range {
    static auto constexpr max_ratio = 16.F;
    static auto constexpr inward_ratio_range = Range{0.F, -max_ratio};
    static auto constexpr outward_ratio_range = Range{0.F, max_ratio};
    static auto constexpr bidirectional_ratio_range =
        Range{-max_ratio, max_ratio};
    static auto constexpr ratio_ranges = std::array<Range const, 3>{
        inward_ratio_range, bidirectional_ratio_range, outward_ratio_range};
    return selected_range<3>(module->params[Controls::DirectionSwitch],
                             ratio_ranges);
  }

  static inline auto ratio(rack::engine::Module const *module, float rotation)
      -> float {
    auto const is_quantized =
        position_of(module->params[Controls::FreeRatioSwitch]) == 0;
    auto const unquantized_ratio = ratio_range(module).scale(rotation);
    return is_quantized ? std::round(unquantized_ratio) : unquantized_ratio;
  }

  class Quantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override {
      return ratio(module, getValue());
    }

    void setDisplayValue(float bounce_ratio) override {
      auto const &range = ratio_range(module);
      auto const rotation = range.normalize(bounce_ratio);
      setValue(rotation);
    }
  };

  static inline void config(rack::engine::Module *module, int knob_id) {
    module->configParam<Quantity>(knob_id, 0.F, 1.F, 0.5F, "Ratio", "x");
  }
};
} // namespace xycloid
} // namespace dhe
