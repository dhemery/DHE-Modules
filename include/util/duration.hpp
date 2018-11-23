#pragma once
#include <functional>
#include <vector>

#include "range.hpp"
#include "sigmoid.hpp"

namespace DHE {

namespace Duration {
auto constexpr short_range{Range{0.001f, 1.f}};
auto constexpr medium_range{Range{0.01f, 10.f}};
auto constexpr long_range{Range{0.1f, 100.f}};

/**
 * Selects a duration by applying a J taper to the proportion
 * and mapping the result onto the range [0.01, 10.0] seconds.
 * <p>
 * The proportion selects a value as follows:
 * <ul>
 * <li>0.0 selects the lower bound of the range.</li>
 * <li>0.5 selects a value approximately 1/10 the upper bound of the
 * range.</li>
 * <li>1.0 selects the upper bound of the range.</li>
 *
 * @param proportion selects the duration
 * @param range the range from which to select the duration
 * @return a function that yields the selected duration
 */
inline auto of(float proportion, Range range = medium_range) -> float {
  // Shapes the J taper to map an input of 0.5 to an output of ~0.1. Thus a
  // proportion of 0.5 will yield a duration of ~1/10 of the range's maximum.
  static auto constexpr knob_curvature{0.8f};

  // Apply the J taper to the proportion.
  auto j_tapered_proportion{sigmoid(proportion, knob_curvature)};

  // Scale the tapered proportion to the desired range.
  return range.scale(j_tapered_proportion);
}
} // namespace Duration
} // namespace DHE
