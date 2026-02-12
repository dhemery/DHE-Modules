#pragma once

#include "components/range.h"

#include <array>
#include <cstdint>

namespace dhe {
enum class DurationRangeId { Short, Medium, Long };
struct DurationRange {
  DurationRange(Range const &);
  enum class Id : std::int8_t { Short, Medium, Long };

  static auto constexpr count = 2;
  static auto labels() -> std::vector<std::string> const &;
  static auto by_id(Id) -> DurationRange const &;

  auto scale(float rotation) -> float;
  auto normalize(float seconds) -> float;

private:
  Range const range_;
};

namespace duration {
static auto short_range = Range{0.001F, 1.F};
static auto medium_range = Range{0.01F, 10.F};
static auto long_range = Range{0.1F, 100.F};

/*
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */
static auto constexpr taper_curvature = 0.8018017F;

auto scale(float rotation, Range range) -> float;

auto normalize(float seconds, Range range) -> float;

static auto constexpr range_count = 3;

static auto ranges =
    std::array<Range, range_count>{short_range, medium_range, long_range};

static auto constexpr labels = std::array<char const *, range_count>{
    "0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};

template <typename R> struct Tapered {
  static constexpr auto scale(float normalized) -> float {
    return duration::scale(normalized, R::range);
  }

  static constexpr auto normalize(float scaled) -> float {
    return duration::normalize(scaled, R::range);
  }
  friend R;
};

} // namespace duration

static auto short_duration_range = DurationRange{duration::short_range};
static auto medium_duration_range = DurationRange{duration::medium_range};
static auto long_duration_range = DurationRange{duration::long_range};

struct ShortDuration : duration::Tapered<ShortDuration> {
  static auto constexpr &range = duration::short_range;
};

struct MediumDuration : duration::Tapered<MediumDuration> {
  static auto constexpr &range = duration::medium_range;
};

struct LongDuration : duration::Tapered<LongDuration> {
  static auto constexpr &range = duration::long_range;
};

struct Duration {
  static auto label(DurationRangeId range_id) -> char const *;

  static auto range(DurationRangeId range_id) -> Range;

  static auto scale(float normalized, DurationRangeId range_id) -> float;

  static auto normalize(float scaled, DurationRangeId range_id) -> float;
};

} // namespace dhe
