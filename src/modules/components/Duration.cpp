#include "modules/components/Duration.h"

namespace DHE {

// Note that each range is of the form [n, 1000n].
Range const Duration::shortRange{0.001f, 1.f};
Range const Duration::mediumRange{0.01f, 10.f};
Range const Duration::longRange{0.1f, 100.f};

/**
 * Each duration range is of the form [n, 1000n]. Given ranges of that form,
 * this curvature tapers the rotation so a knob positioned dead center yields
 * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
 * places).
 */
const float Duration::knobTaperCurvature = 0.8018017;

const std::array<Range const *, 3> Duration::ranges{&shortRange, &mediumRange,
                                                    &longRange};

auto Duration::range(int switchPosition) -> Range const * {
  return ranges[switchPosition];
}

auto Duration::range(float switchPosition) -> Range const * {
  return range(static_cast<int>(switchPosition));
}
} // namespace DHE
