#pragma once
#include "gain.h"

namespace dhe {
template <typename TParam> auto value_of(TParam const &param) -> float {
  return const_cast<TParam &>(param).getValue();
}

template <typename TInput> auto voltage_at(TInput const &input) -> float {
  return const_cast<TInput &>(input).getVoltage();
}

template <typename TParam> auto position_of(TParam const &param) -> int {
  return static_cast<int>(value_of(param));
}

template <typename TParam> auto is_pressed(TParam const &param) -> bool {
  return value_of(param) > 0.5F;
}

template <typename TInput> auto is_high(TInput const &input) -> bool {
  return voltage_at(input) > 1.F;
}

static inline auto rotation(float knob, float cv, float av = 1.F) -> float {
  static auto constexpr rotation_per_volt = 0.1F;
  return knob + cv * rotation_per_volt * Attenuverter::value(av);
}

template <typename TParam, typename TInput>
auto rotation(TParam const &knob, TInput const &cv_input) -> float {
  return rotation(value_of(knob), voltage_at(cv_input));
}

template <typename TParam, typename TInput>
auto rotation(TParam const &knob, TInput const &cv_input, TParam const &av_knob)
    -> float {
  return rotation(value_of(knob), voltage_at(cv_input), value_of(av_knob));
}

} // namespace dhe
