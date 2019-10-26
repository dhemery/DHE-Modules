#include "controls/CommonInputs.h"

#include <array>

namespace dhe {
class Range;
std::array<Range const *, 2> const signalRanges{&bipolarSignalRange, &unipolarSignalRange};
} // namespace dhe
