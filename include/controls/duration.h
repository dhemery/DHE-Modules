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
static auto to_duration_taper(float rotation) -> float {
  return Sigmoid::j_taper(rotation, 0.8f);
}

inline auto knob(const rack::Module *module, int knob_index) -> Knob {
  return Knob{module, knob_index}
      .map(&to_duration_taper)
      .scale_to(medium_range);
}

inline auto ranged_knob(const rack::Module *module, int knob_index,
                        int cv_index, int range_switch_index) -> Knob {
  const auto selected_range = Switch<Range>::three(
      module, range_switch_index, short_range, medium_range, long_range);
  return Knob{module, knob_index, cv_index}
      .map(&to_duration_taper)
      .scale_to(selected_range);
}

} // namespace Duration
} // namespace DHE
