#pragma once

#include "components/sigmoid.h"

#include <array>
#include <string>
#include <vector>

namespace dhe {
namespace internal {
namespace shape {
static auto constexpr labels = std::array<char const *, 2>{"J", "S"};
static auto constexpr unit = "";
} // namespace shape
} // namespace internal

struct JShape {
  static auto constexpr apply(float input, float curvature) -> float {
    return sigmoid::curve(input, curvature);
  }

  static auto constexpr invert(float input, float curvature) -> float {
    return sigmoid::curve(input, -curvature);
  }
};

struct SShape {
  static auto constexpr apply(float input, float curvature) -> float {
    return scale_down(sigmoid::curve(scale_up(input), -curvature));
  }

  static auto constexpr invert(float input, float curvature) -> float {
    return apply(input, -curvature);
  }

private:
  static auto constexpr scale_up(float input) -> float {
    return cx::scale(input, -1.F, 1.F);
  }

  static auto constexpr scale_down(float curved) -> float {
    return cx::normalize(curved, -1.F, 1.F);
  }
};

struct Shape {
  enum class Id { J, S };
  static constexpr auto j = Id::J;
  static constexpr auto s = Id::S;

  static auto constexpr apply(float input, Id id, float curvature) -> float {
    return id == s ? SShape::apply(input, curvature)
                   : JShape::apply(input, curvature);
  }
};

struct Shapes {
  using ValueType = Shape::Id;
  static auto constexpr size = internal::shape::labels.size();
  static auto constexpr stepper_slug = "shape";

  static inline auto labels() -> std::vector<std::string> {
    return {internal::shape::labels.cbegin(), internal::shape::labels.cend()};
  }
};
} // namespace dhe
