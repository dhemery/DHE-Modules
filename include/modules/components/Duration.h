#pragma once

#include <array>

#include "util/range.h"

namespace DHE {
namespace Duration {

// Note that each range is of the form [n, 1000n].
extern Range const shortRange;
extern Range const mediumRange;
extern Range const longRange;

/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */
extern float const knobTaperCurvature;

extern std::array<Range const *, 3> const ranges;
auto range(int switchPosition) -> Range const *;
auto range(float switchPosition) -> Range const *;
} // namespace Duration
} // namespace DHE
