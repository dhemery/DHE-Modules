#pragma once

#include "components/range.h"
#include "components/taper.h"

#include <array>

namespace rack {
namespace engine {
struct Param;
}
} // namespace rack

namespace dhe {

static auto constexpr attenuator_range = Range{0.F, 1.F};
static auto constexpr attenuverter_range = Range{-1.F, 1.F};
static auto constexpr gain_range = Range{0.F, 2.F};

static auto constexpr rotation_range = Range{0.F, 1.F};

static auto constexpr bipolar_signal_range = Range{-5.F, 5.F};
static auto constexpr unipolar_signal_range = Range{0.F, 10.F};

static auto constexpr signal_ranges =
    std::array<Range const *, 2>{&bipolar_signal_range, &unipolar_signal_range};

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

template <typename ItemType, int N, typename ToggleType = rack::engine::Param>
auto selected(ToggleType &toggle, std::array<ItemType, N> const &items)
    -> ItemType {
  return items[position_of(toggle)];
}

template <int N, typename ToggleType = rack::engine::Param>
auto selected_range(ToggleType &toggle,
                    std::array<Range const *, N> const &ranges)
    -> Range const * {
  return selected<Range const *, N>(toggle, ranges);
}

/**
 * Returns the taper selected by the given switch.
 */
template <typename ToggleType>
auto selected_taper(ToggleType &toggle) -> taper::VariableTaper const * {
  return selected<taper::VariableTaper const *, 2>(toggle,
                                                   taper::variable_tapers);
}

template <typename KnobType, typename InputType>
auto rotation(KnobType &knob, InputType &cv_input) -> float {
  static constexpr auto cv_modulation_ratio = 0.1F;
  auto const rotation = rotation_of(knob);
  auto const cv = voltage_at(cv_input);
  auto const modulation = cv * cv_modulation_ratio;
  return rotation + modulation;
}

template <typename KnobType, typename InputType>
auto rotation(KnobType &knob, InputType &cv_input, KnobType &av_knob) -> float {
  static auto constexpr av_modulation_range = Range{-0.1F, 0.1F};
  auto const rotation = rotation_of(knob);
  auto const cv = voltage_at(cv_input);
  auto const av = rotation_of(av_knob);
  auto const cv_modulation_ratio = av_modulation_range.scale(av);
  auto const modulation = cv * cv_modulation_ratio;
  return rotation + modulation;
}

static inline auto tapered_and_scaled_rotation(float rotation,
                                               taper::FixedTaper const &taper,
                                               Range const &range) -> float {
  return range.scale(taper.apply(rotation));
}

template <typename KnobType>
auto tapered_and_scaled_rotation(KnobType &knob, taper::FixedTaper const &taper,
                                 Range const &range) -> float {
  return taperedAndScaledRotation(rotation_of(knob), taper, range);
}

template <typename KnobType, typename InputType>
auto tapered_and_scaled_rotation(KnobType &knob, InputType &cv_input,
                                 taper::FixedTaper const &taper,
                                 Range const &range) -> float {
  return taperedAndScaledRotation(rotation(knob, cv_input), taper, range);
}

template <typename KnobType, typename InputType>
auto tapered_and_scaled_rotation(KnobType &knob, InputType &cv_input,
                                 KnobType &av_knob,
                                 taper::FixedTaper const &taper,
                                 Range const &range) -> float {
  return tapered_and_scaled_rotation(rotation(knob, cv_input, av_knob), taper,
                                     range);
}

} // namespace dhe
