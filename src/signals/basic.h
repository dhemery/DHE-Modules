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

template <typename P> auto rotation_of(P const &knob) -> float {
  return value_of(knob);
}

template <typename P, typename I>
auto rotation_of(P const &knob, I const &cv_input) -> float {
  return value_of(knob) + voltage_at(cv_input) * 0.1F;
}

template <typename P, typename I>
auto rotation_of(P const &knob, I const &cv_input, P const &av_knob) -> float {
  return value_of(knob) +
         voltage_at(cv_input) * 0.1F * Attenuverter::scale(value_of(av_knob));
}

} // namespace dhe
