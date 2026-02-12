#include "signals/duration.h"
#include "components/range.h"
#include "signals/shape.h"

namespace dhe {

DurationRange::DurationRange(Range const &range) : range_{range} {}

auto DurationRange::by_id(Id id) -> DurationRange const & {
  switch (id) {
  case Id::Short:
    return short_duration_range;
  case Id::Medium:
    return medium_duration_range;
  case Id::Long:
    return long_duration_range;
  }
}

auto DurationRange::labels() -> std::vector<std::string> const & {
  static auto const labels =
      std::vector<std::string>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};
  return labels;
}

auto DurationRange::scale(float rotation) -> float {
  return range_.scale(j_shape.apply(rotation, duration::taper_curvature));
}

auto DurationRange::normalize(float seconds) -> float {
  return j_shape.invert(range_.normalize(seconds), duration::taper_curvature);
}

// TODO: Move uses to DurationRange and delete these.
namespace duration {
auto scale(float rotation, Range range) -> float {
  return range.scale(j_shape.apply(rotation, taper_curvature));
}

auto normalize(float seconds, Range range) -> float {
  return j_shape.invert(range.normalize(seconds), taper_curvature);
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
