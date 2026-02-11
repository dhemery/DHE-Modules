#include "signals/shape.h"
#include "components/cxmath.h"
#include "components/sigmoid.h"

namespace dhe {
auto JShape::apply(float input, float curvature) -> float {
  return sigmoid::curve(input, curvature);
}

auto JShape::invert(float input, float curvature) -> float {
  return sigmoid::curve(input, -curvature);
}

auto SShape::apply(float input, float curvature) -> float {
  auto scaled_up = cx::scale(input, -1.F, 1.F);
  auto curved = sigmoid::curve(scaled_up, -curvature);
  return cx::normalize(curved, -1.F, 1.F);
}

auto SShape::invert(float input, float curvature) -> float {
  return apply(input, -curvature);
}

auto Shape::apply(float input, Id id, float curvature) -> float {
  return id == Id::S ? SShape::apply(input, curvature)
                     : JShape::apply(input, curvature);
}

auto Curvature::scale(float normalized) -> float {
  return range.scale(SShape::apply(normalized, taper_curvature));
}

auto Curvature::normalize(float scaled) -> float {
  return SShape::invert(range.normalize(scaled), taper_curvature);
}

} // namespace dhe
