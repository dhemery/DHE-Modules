#include "modules/controls/DurationInputs.h"

namespace dhe {
static auto constexpr shortRange{shortDurationRange};
static auto constexpr mediumRange{mediumDurationRange};
static auto constexpr longRange{longDurationRange};

const std::array<Range const *, 3> durationRanges{&shortRange, &mediumRange, &longRange};
} // namespace dhe
