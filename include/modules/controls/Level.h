#pragma once

#include <array>
#include <functional>

#include "util/range.h"

namespace DHE {
namespace Level {

static Range constexpr unipolarRange{0.f, 10.f};
static Range constexpr bipolarRange{-5.f, 5.f};

extern std::array<Range const *, 2> const ranges;

/**
 * Returns the range for the given range switch position.
 * @param switchPosition the position of the range switch
 */
auto range(int switchPosition) -> Range const *;

/**
 * Creates a function that computes a signal level (in volts) from the given
 * rotation and range functions.
 * @param rotation a function that returns a knob rotation
 * @param range a function that returns a signal level range (in seconds)
 */
auto from(const std::function<float()> &rotation,
          const std::function<Range const *()> &range)
    -> std::function<float()>;

/**
 * Creates a function that computes a signal level (in volts)
 * from the given rotation function and the given range.
 * @param rotation a function that returns a knob rotation
 * @param range the signal level range (in volts)
 */
auto from(const std::function<float()> &rotation, Range const &range)
    -> std::function<float()>;

} // namespace Level
} // namespace DHE
