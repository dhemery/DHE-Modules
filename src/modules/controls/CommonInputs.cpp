#include "modules/controls/CommonInputs.h"

#include "util/Range.h"

#include <array>

namespace dhe {
static auto constexpr bipolar{bipolarSignalRange};
static auto constexpr unipolar{unipolarSignalRange};

auto const signalRanges = std::array<Range const *, 2>{&bipolar, &unipolar};
} // namespace dhe
