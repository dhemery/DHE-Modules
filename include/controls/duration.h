#pragma once

#include "knob.h"
#include "switch.h"
#include "util/range.h"
#include "util/sigmoid.h"

namespace DHE {

namespace Duration {
constexpr auto short_range = Range{0.001f, 1.f};
constexpr auto medium_range = Range{0.01f, 10.f};
constexpr auto long_range = Range{0.1f, 100.f};

/**
 * Selects a duration by applying a J taper to the proportion and mapping the
 * result onto the given range.
 * <p>
 * The proportion selects a value from the range
 * as follows:
 * <ul>
 * <li>0.0 selects the lower bound.</li>
 * <li>0.5 selects a value approximately 1/10 the upper bound.</li>
 * <li>1.0 selects the upper bound.</li>
 *</ul>
 *
 * @param proportion selects the duration
 * @param range the range from which to select the duration
 * @return the selected duration
 */
inline auto duration(float proportion, const Range &range = medium_range)
    -> float {
  // Shapes the J taper to map an input of 0.5 to an output of ~0.1. Thus a
  // proportion of 0.5 will yield a duration of ~1/10 of the range's maximum.
  static constexpr auto curvature = 0.8f;

  // Taper the proportion.
  auto tapered = Sigmoid::j_taper(proportion, curvature);

  // Scale the tapered proportion to the desired range.
  return range.scale(tapered);
}

inline auto knob(const rack::Module *module, int knob_index) -> Knob {
  const auto knob = Knob::plain(module, knob_index);
  return Knob{[knob] { return duration(knob()); }};
}

inline auto ranged_knob(const rack::Module *module, int knob_index,
                        int cv_index, int switch_index) -> Knob {
  const auto knob = Knob::with_cv(module, knob_index, cv_index);
  const auto range = Switch<Range>::three(module, switch_index, short_range,
                                          medium_range, long_range);
  return Knob{[knob, range] { return duration(knob(), range()); }};
}

} // namespace Duration
} // namespace DHE
