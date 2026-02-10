#pragma once

#include "signals/duration.h"
#include "components/range.h"
#include "signals/shape.h"

#include <array>

namespace dhe {

namespace duration {

/*
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */

auto scale(float rotation, Range range) -> float {
  return range.scale(JShape::apply(rotation, taper_curvature));
}

auto normalize(float seconds, Range range) -> float {
  return JShape::invert(range.normalize(seconds), taper_curvature);
}

} // namespace duration

auto Duration::label(DurationRangeId id) -> char const * {
  return duration::labels[static_cast<size_t>(id)];
}

auto Duration::range(DurationRangeId id) -> Range {
  return duration::ranges[static_cast<size_t>(id)];
}

auto Duration::scale(float normalized, DurationRangeId range_id) -> float {
  return duration::scale(normalized, range(range_id));
}

auto Duration::normalize(float scaled, DurationRangeId range_id) -> float {
  return duration::normalize(scaled, range(range_id));
}

} // namespace dhe
