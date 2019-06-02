#include "modules/controls/Inputs.h"

#include "util/Range.h"

#include <array>

namespace dhe {
auto const signalRanges = std::array<Range const *, 2>{&bipolarSignalRange, &unipolarSignalRange};
}
