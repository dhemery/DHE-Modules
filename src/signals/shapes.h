#pragma once

#include "components/sigmoid.h"

#include <vector>

namespace dhe {

struct Curvature {
  /**
   * This curvature creates a gentle inverted S taper, increasing sensitivity in
   * the middle of the knob rotation and decreasing sensitivity toward the
   * extremes.
   */
  static auto constexpr knob_taper_curvature = -0.65F;

  static constexpr auto value(float rotation) -> float {
    return sigmoid::safe_curvature_range.clamp(
        sigmoid::curve(sigmoid::domain.scale(rotation), knob_taper_curvature));
  }

  static inline auto rotation(float curvature) -> float {
    auto const sigmoid_clamped_curvature =
        sigmoid::safe_curvature_range.clamp(curvature);
    // Unexpected, but true: Negating the taper curvature inverts the taper.
    auto const sigmoid_scaled_rotation =
        sigmoid::curve(sigmoid_clamped_curvature, -knob_taper_curvature);
    return sigmoid::domain.normalize(sigmoid_scaled_rotation);
  }
};

struct Shapes {
  using ItemType = sigmoid::Taper;
  using PositionType = int;

  enum { J, S };
  static auto constexpr unit = "";
  static auto constexpr stepper_slug = "shape";

  static inline auto items() -> std::vector<ItemType> const & {
    static auto const items =
        std::vector<ItemType>{sigmoid::j_taper, sigmoid::s_taper};
    return items;
  }

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"J", "S"};
    return labels;
  }

  static inline auto select(int selection) -> ItemType const & {
    return items()[selection];
  }

  static inline auto value(float rotation, int selection) -> float {
    return select(selection).apply(rotation);
  }
};
} // namespace dhe
