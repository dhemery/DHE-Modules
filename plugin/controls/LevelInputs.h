#pragma once

#include "CommonInputs.h"
#include "components/Range.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>

namespace dhe {
static inline auto levelRange(rack::engine::Param const &toggle) -> Range const * {
  return selectedRange<2>(toggle, signalRanges);
}

static inline auto level(rack::engine::Param const &knob, Range const &range) -> float {
  return range.scale(paramValue(knob));
}

static inline auto selectableLevel(rack::engine::Param const &knob, rack::engine::Param const &toggle) -> float {
  return levelRange(toggle)->scale(paramValue(knob));
}

static inline auto selectableLevel(rack::engine::Param const &knob, rack::engine::Input const &cvInput,
                                   rack::engine::Param const &toggle) -> float {
  return levelRange(toggle)->scale(rotation(knob, cvInput));
}

static inline auto selectableLevel(rack::engine::Param const &knob, rack::engine::Input const &cvInput,
                                   rack::engine::Param const &avKnob, rack::engine::Param const &toggle) -> float {
  return levelRange(toggle)->scale(rotation(knob, cvInput, avKnob));
}

} // namespace dhe
