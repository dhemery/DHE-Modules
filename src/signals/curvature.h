#pragma once

#include "components/sigmoid.h"

namespace dhe {

struct Curvature {
  struct KnobMapper;
  static auto constexpr default_rotation = 0.5F;
  static auto constexpr unit = "";

  static constexpr auto curvature(float normalized) -> float {
    return sigmoid::safe_curvature_range.clamp(
        sigmoid::curve(sigmoid::domain.scale(normalized), taper_curvature));
  }

  static inline auto normalize(float curvature) -> float {
    auto const sigmoid_clamped_curvature =
        sigmoid::safe_curvature_range.clamp(curvature);
    // Unexpected, but true: Negating the taper curvature inverts the taper.
    auto const sigmoid_scaled_rotation =
        sigmoid::curve(sigmoid_clamped_curvature, -taper_curvature);
    return sigmoid::domain.normalize(sigmoid_scaled_rotation);
  }

private:
  /**
   * This curvature gives a curvature knob a gentle inverted S taper, increasing
   * sensitivity in the middle of the knob normalize and decreasing sensitivity
   * toward the extremes.
   */
  static auto constexpr taper_curvature = -0.65F;
};

struct Curvature::KnobMapper {
  static inline auto scale(float normalized) -> float {
    return Curvature::curvature(normalized);
  }

  static inline auto normalize(float curvature) -> float {
    return Curvature::normalize(curvature);
  }
};
} // namespace dhe
