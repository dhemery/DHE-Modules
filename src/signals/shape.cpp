#include "signals/shape.h"
#include "components/cxmath.h"
#include "components/sigmoid.h"

namespace dhe {
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

auto Shape::by_id(Id id) -> Shape const & {
  return id == Id::J ? j_shape : s_shape;
}

auto Curvature::scale(float normalized) -> float {
  return range.scale(s_shape.apply(normalized, taper_curvature));
}

auto Curvature::normalize(float scaled) -> float {
  return s_shape.invert(range.normalize(scaled), taper_curvature);
}

} // namespace dhe
