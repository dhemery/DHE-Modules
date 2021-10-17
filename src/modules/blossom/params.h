#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "components/sigmoid.h"
#include "params/common-config.h"
#include "params/float-params.h"

#include "rack.hpp"

namespace dhe {
namespace blossom {

struct BounceRatio {
  static auto constexpr range = Range{1.F, 17.F};

  // TODO: Make panel set_is_quantized(v) when ratio switch changes, so this
  //  struct doesn't have to know about that param.
  struct Quantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      auto const rotation = getValue();
      auto const unquantized_ratio = range.scale(rotation);
      auto const is_quantized =
          position_of(module->params[Params::BounceRatioMode]) == 0;
      return is_quantized ? std::round(unquantized_ratio) : unquantized_ratio;
    }

    void setDisplayValue(float bounce_ratio) override {
      auto const rotation = range.normalize(bounce_ratio);
      setValue(rotation);
    }
  };

  static inline void config(rack::engine::Module *module, int id) {
    module->configParam<Quantity>(id, 0.F, 1.F, 0.5, "Ratio", "x");
  }

  static inline auto value(float rotation) -> float {
    return range.scale(rotation);
  }
};

struct Phase : LinearFloat<Phase> {
  static auto constexpr display_range = Range{-180.F, 180.F};
  static auto constexpr default_display_value = 0.F;
  static auto constexpr unit = "˚";

  static inline auto value(float rotation) -> float { return rotation - 0.5F; }
};

struct SpinSpeed {
  static auto constexpr range = Range{-10.F, 10.F};
  static auto constexpr initial_spin_hz{1.F};
  static auto constexpr taper_curvature = -0.8F;
  static auto constexpr taper =
      sigmoid::s_taper_with_curvature(taper_curvature);
  static auto constexpr spin_to_rotation(float spin) -> float {
    return taper.invert(range.normalize(spin));
  }
  static auto constexpr rotation_to_spin(float rotation) -> float {
    return range.scale(taper.apply(rotation));
  }

  struct Quantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override {
      return rotation_to_spin(getValue());
    }

    void setDisplayValue(float spin) override {
      setValue(spin_to_rotation(spin));
    }
  };

  static inline void config(rack::engine::Module *module, int knob_id) {
    static auto constexpr initial_rotation = spin_to_rotation(initial_spin_hz);

    module->configParam<Quantity>(knob_id, 0.F, 1.F, initial_rotation, "Speed",
                                  " Hz");
  }
};

} // namespace blossom
} // namespace dhe
