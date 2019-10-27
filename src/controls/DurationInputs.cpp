#include "controls/DurationInputs.h"

#include <array>

namespace dhe {

class Range;

const std::array<Range const *, 3> durationRanges{&shortDurationRange, &mediumDurationRange, &longDurationRange};
} // namespace dhe
