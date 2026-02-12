#include "signals/duration.h"
#include "components/range.h"
#include "signals/shape.h"

#include <string>
#include <vector>

namespace dhe {

/*
 * Each duration taper's range is of the form [n, 1000n]. Given ranges of that
 * form, this curvature tapers the rotation so a knob positioned dead center
 * yields a duration equal to 1/10 of the range's upper bound (to within 7
 * decimal places).
 */
static auto constexpr taper_curvature = 0.8018017F;

DurationTaper::DurationTaper(Range const &range) : range_{range} {}

auto DurationTaper::by_id(Id id) -> DurationTaper const & {
  switch (id) {
  case Id::Short:
    return short_duration_range;
  case Id::Medium:
    return medium_duration_range;
  case Id::Long:
    return long_duration_range;
  }
}

auto DurationTaper::labels() -> std::vector<std::string> const & {
  static auto const labels =
      std::vector<std::string>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};
  return labels;
}

auto DurationTaper::label(Id id) -> std::string {
  return labels()[static_cast<int>(id)];
}

auto DurationTaper::scale(float rotation) const -> float {
  return range_.scale(j_shape.apply(rotation, taper_curvature));
}

auto DurationTaper::normalize(float seconds) const -> float {
  return j_shape.invert(range_.normalize(seconds), taper_curvature);
}

auto DurationTaper::range() const -> Range const & { return range_; }

} // namespace dhe
