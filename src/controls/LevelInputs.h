#pragma once

#include "CommonInputs.h"
#include "components/Range.h"

namespace dhe {
template <typename ToggleType>
auto levelRange(ToggleType &toggle) -> Range const * {
  return selectedRange<2>(toggle, signalRanges);
}

static inline auto level(float rotation, Range const &range) -> float {
  return range.scale(rotation);
}

template <typename KnobType>
static inline auto level(KnobType &knob, Range const &range) -> float {
  return level(rotationOf(knob), range);
}

template <typename KnobType, typename ToggleType>
static inline auto selectableLevel(KnobType &knob, ToggleType &toggle)
    -> float {
  return level(rotationOf(knob), *levelRange(toggle));
}

template <typename KnobType, typename InputType, typename ToggleType>
static inline auto selectableLevel(KnobType &knob, InputType &cvInput,
                                   ToggleType &toggle) -> float {
  return level(rotation(knob, cvInput), *levelRange(toggle));
}

template <typename KnobType, typename InputType, typename ToggleType>
static inline auto selectableLevel(KnobType &knob, InputType &cvInput,
                                   KnobType &avKnob, ToggleType &toggle)
    -> float {
  return level(rotation(knob, cvInput, avKnob), *levelRange(toggle));
}

} // namespace dhe
