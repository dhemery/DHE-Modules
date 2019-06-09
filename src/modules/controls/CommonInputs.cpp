#include "modules/controls/CommonInputs.h"

#include "modules/components/Range.h"

#include <array>

namespace dhe {
std::array<Range const *, 2> const signalRanges{&bipolarSignalRange, &unipolarSignalRange};
} // namespace dhe
