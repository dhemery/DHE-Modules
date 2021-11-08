#pragma once
#include "gain.h"

#include "components/range.h"
#include "components/sigmoid.h"

#include <array>

namespace dhe {
template <typename TParam> auto value_of(TParam &param) -> float {
  return param.getValue();
}

template <typename TParam> auto value_of(TParam const &param) -> float {
  return value_of(const_cast<TParam &>(param));
}

template <typename TInput> auto voltage_at(TInput &input) -> float {
  return input.getVoltage();
}

template <typename TInput> auto voltage_at(TInput const &input) -> float {
  return voltage_at(const_cast<TInput &>(input));
}

template <typename TParam> auto position_of(TParam &param) -> int {
  return static_cast<int>(value_of(param));
}

template <typename TParam> auto is_pressed(TParam &param) -> bool {
  return value_of(param) > 0.5F;
}

template <typename TInput> auto is_high(TInput &input) -> bool {
  return voltage_at(input) > 1.F;
}

template <typename TItem, int N, typename TParam>
auto selected(TParam &param, std::array<TItem, N> const &items)
    -> TItem const & {
  return items[position_of(param)];
}

template <int N, typename TParam>
auto selected_range(TParam &param, std::array<Range const, N> const &ranges)
    -> Range {
  return selected<Range const, N>(param, ranges);
}

/**
 * Returns the taper selected by the given switch.
 */
template <typename TParam>
auto selected_taper(TParam &param) -> sigmoid::Taper const & {
  return selected<sigmoid::Taper const, 2>(param, sigmoid::tapers);
}

template <typename TParam> auto rotation_of(TParam &param) -> float {
  return value_of(param);
}

template <typename TPparam, typename TInput>
auto rotation(TPparam &knob, TInput &cv_input) -> float {
  static auto constexpr rotation_per_volt = 0.1F;
  auto const rotation = rotation_of(knob);
  auto const cv = voltage_at(cv_input);
  return rotation + cv * rotation_per_volt;
}

template <typename TParam, typename TInput>
auto rotation(TParam &knob, TInput &cv_input, TParam &av_knob) -> float {
  static auto constexpr rotation_per_volt = 0.1F;
  auto const rotation = rotation_of(knob);
  auto const cv = voltage_at(cv_input);
  auto const av = Attenuverter::value(rotation_of(av_knob));
  return rotation + cv * rotation_per_volt * av;
}

template <typename TParam, typename TInput>
auto tapered_and_scaled_rotation(TParam &knob, TInput &cv_input,
                                 TParam &av_knob, sigmoid::Taper const &taper,
                                 Range range) -> float {
  return tapered_and_scaled_rotation(rotation(knob, cv_input, av_knob), taper,
                                     range);
}

} // namespace dhe
