#pragma once

#include "common-inputs.h"
#include "components/range.h"

namespace dhe {
template <typename ToggleType>
auto level_range(ToggleType &toggle) -> Range const & {
  return selected_range<2>(toggle, signal_ranges);
}

static inline auto level(float rotation, Range const &range) -> float {
  return range.scale(rotation);
}

template <typename KnobType>
static inline auto level(KnobType &knob, Range const &range) -> float {
  return level(rotation_of(knob), range);
}

template <typename KnobType, typename ToggleType>
static inline auto selectable_level(KnobType &knob, ToggleType &toggle)
    -> float {
  return level(rotation_of(knob), level_range(toggle));
}

template <typename KnobType, typename InputType, typename ToggleType>
static inline auto selectable_level(KnobType &knob, InputType &cv_input,
                                    ToggleType &toggle) -> float {
  return level(rotation(knob, cv_input), level_range(toggle));
}

template <typename KnobType, typename InputType, typename ToggleType>
static inline auto selectable_level(KnobType &knob, InputType &cv_input,
                                    KnobType &av_knob, ToggleType &toggle)
    -> float {
  return level(rotation(knob, cv_input, av_knob), level_range(toggle));
}

} // namespace dhe
