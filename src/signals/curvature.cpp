#include "signals/curvature.h"
#include "components/range.h"
#include "signals/shape.h"

namespace dhe {
namespace {
auto const curvature_signal_range = Range{-0.9999F, 0.9999F};
/**
 * This curvature gives a curvature knob a gentle inverted S taper, increasing
 * sensitivity in the middle of the knob normalize and decreasing sensitivity
 * toward the extremes.
 */
auto constexpr curvature_signal_curvature = 0.65F;
} // namespace

namespace curvature {
auto scale(float normalized) -> float {
  return curvature_signal_range.scale(
      s_shape.apply(normalized, curvature_signal_curvature));
}

auto normalize(float scaled) -> float {
  return s_shape.invert(curvature_signal_range.normalize(scaled),
                        curvature_signal_curvature);
}
} // namespace curvature

} // namespace dhe
