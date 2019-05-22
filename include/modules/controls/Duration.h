#pragma once

#include <array>
#include <functional>

#include "util/range.h"

namespace DHE {
namespace Duration {

static Range constexpr shortRange{0.001f, 1.f};
static Range constexpr mediumRange{0.01f, 10.f};
static Range constexpr longRange{0.1f, 100.f};

/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */
extern float const knobTaperCurvature;

extern std::array<Range const *, 3> const ranges;

/**
 * Returns the range for the given range switch position.
 * @param switchPosition the position of the range switch
 */
auto range(float switchPosition) -> Range const *;

/**
 * Creates a function that computes a duration (in seconds) from the given
 * rotation and range functions.
 * @param rotation a function that returns a knob rotation
 * @param range a function that returns a duration range (in seconds)
 */
auto from(const std::function<float()> &rotation,
          const std::function<Range const *()> &range)
    -> std::function<float()>;

/**
 * Creates a function that computes a medium-range duration (in seconds)
 * from the given rotation.
 * @param rotation a function that returns a knob rotation
 * @param range a function that returns a duration range (in seconds)
 */
auto from(const std::function<float()> &rotation) -> std::function<float()>;

} // namespace Duration
} // namespace DHE
