#pragma once

#include "components/sigmoid.h"

#include <vector>

namespace dhe {

struct Curvature {
  struct KnobMapper;
  static auto constexpr unit = "";

  static constexpr auto curvature(float rotation) -> float {
    return sigmoid::safe_curvature_range.clamp(
        sigmoid::curve(sigmoid::domain.scale(rotation), taper_curvature));
  }

  static inline auto rotation(float curvature) -> float {
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
   * sensitivity in the middle of the knob rotation and decreasing sensitivity
   * toward the extremes.
   */
  static auto constexpr taper_curvature = -0.65F;
};

struct Curvature::KnobMapper {
  auto to_display_value(float rotation) const -> float {
    return curvature(rotation);
  }

  auto to_rotation(float curvature) const -> float {
    return rotation(curvature);
  }
};

enum class Shape { J, S };

struct Shapes {
  using Selection = Shape;
  static auto constexpr unit = "";
  static auto constexpr stepper_slug = "shape";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"J", "S"};
    return labels;
  }

  static inline auto taper(Shape shape) -> sigmoid::Taper const & {
    static auto const tapers =
        std::vector<sigmoid::Taper>{sigmoid::j_taper, sigmoid::s_taper};
    return tapers[(int)shape];
  }

  static inline auto curvature(float rotation, Shape shape) -> float {
    return taper(shape).apply(rotation);
  }
};
} // namespace dhe
