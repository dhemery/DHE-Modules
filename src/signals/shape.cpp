#include "signals/shape.h"
#include "components/sigmoid.h"

namespace dhe {

auto JShape::apply(float input, float curvature) -> float {
  return sigmoid::curve(input, curvature);
}

auto JShape::invert(float input, float curvature) -> float {
  return sigmoid::curve(input, -curvature);
}

auto SShape::apply(float input, float curvature) -> float {
  return scale_down(sigmoid::curve(scale_up(input), -curvature));
}

auto SShape::invert(float input, float curvature) -> float {
  return apply(input, -curvature);
}

auto SShape::scale_up(float input) -> float {
  return cx::scale(input, -1.F, 1.F);
}

auto SShape::scale_down(float curved) -> float {
  return cx::normalize(curved, -1.F, 1.F);
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
