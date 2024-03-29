#pragma once

namespace dhe {

static constexpr auto pi{3.14159265359F};
static constexpr auto tau{2.F * pi};

/**
 * constexpr versions of common math functions.
 */
namespace cx {
template <typename T> static constexpr auto max(T a, T b) -> T {
  return a > b ? a : b;
}

template <typename T> static constexpr auto min(T a, T b) -> T {
  return a < b ? a : b;
}

template <typename T> static constexpr auto abs(T t) -> T {
  return t < 0 ? -t : t;
}

static constexpr auto normalize(float scaled, float lower_bound,
                                float upper_bound) -> float {
  return (scaled - lower_bound) / (upper_bound - lower_bound);
}

static constexpr auto scale(float normalized, float lower_bound,
                            float upper_bound) -> float {
  return normalized * (upper_bound - lower_bound) + lower_bound;
}

static constexpr auto clamp(float value, float lower_bound, float upper_bound)
    -> float {
  return max(min(value, upper_bound), lower_bound);
}

} // namespace cx
} // namespace dhe
