#pragma once

#include "CommonInputs.h"
#include "util/Range.h"

#include <array>

namespace dhe {
static inline auto levelRange(rack::engine::Module const *module, int switchId) -> Range const * {
  return selectedRange<2>(module, switchId, signalRanges);
}

static inline auto level(rack::engine::Module const *module, int knobId, Range const &range) -> float {
  return scaledRotation(module, knobId, range);
}

static inline auto selectableLevel(rack::engine::Module const *module, int knobId, int cvId, int switchId) -> float {
  auto const range = levelRange(module, switchId);
  return scaledRotation(module, knobId, cvId, *range);
}

static inline auto selectableLevel(rack::engine::Module const *module, int knobId, int cvId, int avId, int switchId)
    -> float {
  auto const range = levelRange(module, switchId);
  return scaledRotation(module, knobId, cvId, avId, *range);
}

} // namespace dhe
