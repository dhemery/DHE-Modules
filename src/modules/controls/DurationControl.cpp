#include <array>

#include "modules/controls/DurationControl.h"
#include "util/sigmoid.h"

namespace DHE {

// Note that each range is of the form [n, 1000n].
const Range DurationControl::short_range{0.001f, 1.f};
const Range DurationControl::medium_range{0.01f, 10.f};
const Range DurationControl::long_range{0.1f, 100.f};

ConstantParam DurationControl::constantMediumDurationRangeSwitch{1.f};

const std::array<Range const *, 3> DurationControl::ranges{
    &short_range, &medium_range, &long_range};

auto DurationControl::seconds() -> float {
  /**
   * Each duration range is of the form [n, 1000n]. Given ranges of that form,
   * this curvature tapers the rotation so a knob positioned dead center yields
   * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
   * places).
   */
  static auto constexpr curvature = 0.8018017;
  auto const tapered = Sigmoid::j_shape.taper(rotation(), curvature);
  auto const range_index = static_cast<int>(rangeSwitch.getValue());
  auto const *range = ranges[range_index];
  return range->scale(tapered);
}

} // namespace DHE
