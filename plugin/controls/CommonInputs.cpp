#include "CommonInputs.h"

#include "Range.h"

#include <array>

namespace dhe {
std::array<Range const *, 2> const signalRanges{&bipolarSignalRange, &unipolarSignalRange};
} // namespace dhe
