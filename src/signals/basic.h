#pragma once
#include "gain.h"

namespace dhe {
template <typename V = float, typename P> auto value_of(P const &param) -> V {
  return static_cast<V>(const_cast<P &>(param).getValue());
}

template <typename I> auto voltage_at(I const &input) -> float {
  return const_cast<I &>(input).getVoltage();
}

template <typename P> auto is_pressed(P const &param) -> bool {
  return value_of<bool>(param);
}

template <typename I> auto is_high(I const &input) -> bool {
  return voltage_at(input) > 1.F;
}

static inline auto rotation_of(float rotation, float cv,
                               float cv_multiplier = 1.F) -> float {
  static auto constexpr rotation_per_volt = 0.1F;
  auto const modulation = cv * rotation_per_volt * cv_multiplier;
  return rotation + modulation;
}

template <typename P> auto rotation_of(P const &knob) -> float {
  return value_of(knob);
}

template <typename P, typename I>
auto rotation_of(P const &knob, I const &cv_input) -> float {
  auto const rotation = value_of(knob);
  auto const cv = voltage_at(cv_input);
  return rotation_of(rotation, cv);
}

template <typename P, typename I>
auto rotation_of(P const &knob, I const &cv_input, P const &av_knob) -> float {
  auto const rotation = value_of(knob);
  auto const cv = voltage_at(cv_input);
  auto const av_rotation = value_of(av_knob);
  auto const multiplier = Attenuverter::scale(av_rotation);
  return rotation_of(rotation, cv, multiplier);
}

} // namespace dhe
