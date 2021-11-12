#pragma once

#include "components/sigmoid.h"

#include <vector>

namespace dhe {

struct Curvature {
  struct KnobMapper;
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
  auto scale(float normalized) const -> float {
    return Curvature::curvature(normalized);
  }

  auto normalize(float curvature) const -> float {
    return Curvature::normalize(curvature);
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

  static inline auto taper(float normalized, Shape shape, float curvature)
      -> float {
    return taper(shape).apply(normalized, curvature);
  }

private:
  static inline auto taper(Shape shape) -> sigmoid::Taper const & {
    static auto const tapers =
        std::vector<sigmoid::Taper>{sigmoid::j_taper, sigmoid::s_taper};
    return tapers[static_cast<int>(shape)];
  }
};
} // namespace dhe
