#include "modules/controls/CommonInputs.h"

#include "util/Range.h"

#include <array>

namespace dhe {
std::array<Range const *, 2> const signalRanges{&bipolarSignalRange, &unipolarSignalRange};
} // namespace dhe
