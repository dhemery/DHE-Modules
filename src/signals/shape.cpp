#include "signals/shape.h"
#include "components/cxmath.h"
#include "components/sigmoid.h"

namespace dhe {
auto Shape::get(Id id) -> Shape const & {
  if (id == Id::J) {
    return j_shape;
  }
  return s_shape;
}

auto Shape::labels() -> std::vector<std::string> const & {
  static auto const labels = std::vector<std::string>{"J", "S"};
  return labels;
}

auto JShape::apply(float input, float curvature) const -> float {
  return sigmoid::curve(input, curvature);
}

auto JShape::invert(float input, float curvature) const -> float {
  return sigmoid::curve(input, -curvature);
}

auto SShape::apply(float input, float curvature) const -> float {
  auto scaled_up = cx::scale(input, -1.F, 1.F);
  auto curved = sigmoid::curve(scaled_up, -curvature);
  return cx::normalize(curved, -1.F, 1.F);
}

auto SShape::invert(float input, float curvature) const -> float {
  return apply(input, -curvature);
}
} // namespace dhe
