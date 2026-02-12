#include "signals/duration.h"
#include "components/range.h"
#include "signals/shape.h"

#include <string>
#include <utility>
#include <vector>

namespace dhe {

/*
 * Each duration curve's range is of the form [n, 1000n]. Given ranges of that
 * form, this curvature tapers the rotation so a knob positioned dead center
 * yields a duration equal to 1/10 of the range's upper bound (to within 7
 * decimal places).
 */
static auto constexpr taper_curvature = 0.8018017F;

DurationCurve::DurationCurve(Range range, std::string label)
    : range_{range}, label_{std::move(label)} {}

auto DurationCurve::by_id(Id id) -> DurationCurve const & {
  switch (id) {
  case Id::Short:
    return short_duration_curve;
  case Id::Medium:
  default:
    return medium_duration_curve;
  case Id::Long:
    return long_duration_curve;
  }
}

auto DurationCurve::labels() -> std::vector<std::string> const & {
  static auto const labels = std::vector<std::string>{
      short_duration_curve.label(),
      medium_duration_curve.label(),
      long_duration_curve.label(),
  };
  return labels;
}

auto DurationCurve::scale(float rotation) const -> float {
  return range_.scale(j_shape.apply(rotation, taper_curvature));
}

auto DurationCurve::normalize(float seconds) const -> float {
  return j_shape.invert(range_.normalize(seconds), taper_curvature);
}

auto DurationCurve::range() const -> Range const & { return range_; }

auto DurationCurve::label() const -> std::string const & { return label_; }
} // namespace dhe
