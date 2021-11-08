#pragma once
#include "gain.h"

#include "components/range.h"
#include "components/sigmoid.h"

#include <array>

namespace dhe {
template <typename ParamType> auto value_of(ParamType &param) -> float {
  return param.getValue();
}

template <typename ParamType> auto value_of(ParamType const &param) -> float {
  return value_of(const_cast<ParamType &>(param));
}

template <typename KnobType> auto rotation_of(KnobType &knob) -> float {
  return value_of(knob);
}

template <typename InputType> auto voltage_at(InputType &input) -> float {
  return input.getVoltage();
}

template <typename InputType> auto voltage_at(InputType const &input) -> float {
  return voltage_at(const_cast<InputType &>(input));
}

template <typename ToggleType> auto position_of(ToggleType &toggle) -> int {
  return static_cast<int>(value_of(toggle));
}

template <typename ButtonType> auto is_pressed(ButtonType &button) -> bool {
  return value_of(button) > 0.5F;
}

template <typename T> auto is_high(T &input) -> bool {
  return voltage_at(input) > 1.F;
}

template <typename ItemType, int N, typename ToggleType>
auto selected(ToggleType &toggle, std::array<ItemType, N> const &items)
    -> ItemType const & {
  return items[position_of(toggle)];
}

template <int N, typename ToggleType>
auto selected_range(ToggleType &toggle,
                    std::array<Range const, N> const &ranges) -> Range {
  return selected<Range const, N>(toggle, ranges);
}

/**
 * Returns the taper selected by the given switch.
 */
template <typename ToggleType>
auto selected_taper(ToggleType &toggle) -> sigmoid::Taper const & {
  return selected<sigmoid::Taper const, 2>(toggle, sigmoid::tapers);
}

template <typename KnobType, typename InputType>
auto rotation(KnobType &knob, InputType &cv_input) -> float {
  static auto constexpr rotation_per_volt = 0.1F;
  auto const rotation = rotation_of(knob);
  auto const cv = voltage_at(cv_input);
  return rotation + cv * rotation_per_volt;
}

template <typename KnobType, typename InputType>
auto rotation(KnobType &knob, InputType &cv_input, KnobType &av_knob) -> float {
  static auto constexpr rotation_per_volt = 0.1F;
  auto const rotation = rotation_of(knob);
  auto const cv = voltage_at(cv_input);
  auto const av = Attenuverter::value(rotation_of(av_knob));
  return rotation + cv * rotation_per_volt * av;
}

template <typename KnobType, typename InputType>
auto tapered_and_scaled_rotation(KnobType &knob, InputType &cv_input,
                                 KnobType &av_knob, sigmoid::Taper const &taper,
                                 Range range) -> float {
  return tapered_and_scaled_rotation(rotation(knob, cv_input, av_knob), taper,
                                     range);
}

} // namespace dhe
