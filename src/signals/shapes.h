#pragma once

#include "components/sigmoid.h"

#include <string>
#include <vector>

namespace dhe {
enum class Shape { J, S };

struct Shapes {
  using value_type = Shape; // NOLINT
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
