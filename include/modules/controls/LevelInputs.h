#pragma once

#include "CommonInputs.h"
#include "modules/components/Range.h"

#include <array>
#include <engine/Module.hpp>

namespace dhe {
static inline auto levelRange(rack::engine::Module const *module, int switchId) -> Range const * {
  return selectedRange<2>(module->params[switchId], signalRanges);
}

static inline auto level(rack::engine::Module const *module, int knobId, Range const &range) -> float {
  return range.scale(paramValue(module->params[knobId]));
}

static inline auto selectableLevel(rack::engine::Module const *module, int knobId, int cvId, int switchId) -> float {
  auto const range = levelRange(module, switchId);
  return (*range).scale(rotation(module->params[knobId], module->inputs[cvId]));
}

static inline auto selectableLevel(rack::engine::Module const *module, int knobId, int cvId, int avId, int switchId)
    -> float {
  auto const range = levelRange(module, switchId);
  return range->scale(rotation(module->params[knobId], module->inputs[cvId], module->params[avId]));
}

} // namespace dhe
