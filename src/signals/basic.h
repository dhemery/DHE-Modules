#pragma once
#include "gain.h"

namespace dhe {
template <typename TValue = float, typename TParam>
auto value_of(TParam const &param) -> TValue {
  return static_cast<TValue>(const_cast<TParam &>(param).getValue());
}

template <typename TInput> auto voltage_at(TInput const &input) -> float {
  return const_cast<TInput &>(input).getVoltage();
}

template <typename TParam> auto is_pressed(TParam const &param) -> bool {
  return value_of<bool>(param);
}

template <typename TInput> auto is_high(TInput const &input) -> bool {
  return voltage_at(input) > 1.F;
}

static inline auto rotation_of(float rotation, float cv,
                               float cv_multiplier = 1.F) -> float {
  static auto constexpr rotation_per_volt = 0.1F;
  auto const modulation = cv * rotation_per_volt * cv_multiplier;
  return rotation + modulation;
}

template <typename TParam> auto rotation_of(TParam const &knob) -> float {
  return value_of(knob);
}

template <typename TParam, typename TInput>
auto rotation_of(TParam const &knob, TInput const &cv_input) -> float {
  auto const rotation = value_of(knob);
  auto const cv = voltage_at(cv_input);
  return rotation_of(rotation, cv);
}

template <typename TParam, typename TInput>
auto rotation_of(TParam const &knob, TInput const &cv_input,
                 TParam const &av_knob) -> float {
  auto const rotation = value_of(knob);
  auto const cv = voltage_at(cv_input);
  auto const av_rotation = value_of(av_knob);
  auto const multiplier = Attenuverter::scale(av_rotation);
  return rotation_of(rotation, cv, multiplier);
}

} // namespace dhe
