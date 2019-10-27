#pragma once

#include "CommonInputs.h"
#include "components/Range.h"

namespace dhe {
template <typename ToggleType> auto levelRange(ToggleType &toggle) -> Range const * {
  return selectedRange<2>(toggle, signalRanges);
}

template <typename KnobType> static inline auto level(KnobType &knob, Range const &range) -> float {
  return range.scale(rotationOf(knob));
}

template <typename KnobType, typename ToggleType>
static inline auto selectableLevel(KnobType &knob, ToggleType &toggle) -> float {
  return levelRange(toggle)->scale(rotationOf(knob));
}

template <typename KnobType, typename InputType, typename ToggleType>
static inline auto selectableLevel(KnobType &knob, InputType &cvInput, ToggleType &toggle) -> float {
  return levelRange(toggle)->scale(rotation(knob, cvInput));
}

template <typename KnobType, typename InputType, typename ToggleType>
static inline auto selectableLevel(KnobType &knob, InputType &cvInput, KnobType &avKnob, ToggleType &toggle) -> float {
  return levelRange(toggle)->scale(rotation(knob, cvInput, avKnob));
}

} // namespace dhe
