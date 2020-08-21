#pragma once

#include "components/Range.h"
#include "components/Taper.h"

#include <array>

namespace rack {
namespace engine {
struct Param;
}
} // namespace rack

namespace dhe {

static auto constexpr attenuatorRange = Range{0.F, 1.F};
static auto constexpr attenuverterRange = Range{-1.F, 1.F};
static auto constexpr gainRange = Range{0.F, 2.F};

static auto constexpr rotationRange = Range{0.F, 1.F};

static auto constexpr bipolarSignalRange = Range{-5.F, 5.F};
static auto constexpr unipolarSignalRange = Range{0.F, 10.F};

static auto const signalRanges =
    std::array<Range const *, 2>{&bipolarSignalRange, &unipolarSignalRange};

template <typename ParamType> auto valueOf(ParamType &param) -> float {
  return param.getValue();
}

template <typename ParamType> auto valueOf(ParamType const &param) -> float {
  return valueOf(const_cast<ParamType &>(param));
}

template <typename KnobType> auto rotationOf(KnobType &knob) -> float {
  return valueOf(knob);
}

template <typename InputType> auto voltageAt(InputType &input) -> float {
  return input.getVoltage();
}

template <typename InputType> auto voltageAt(InputType const &input) -> float {
  return voltageAt(const_cast<InputType &>(input));
}

template <typename ToggleType> auto positionOf(ToggleType &toggle) -> int {
  return static_cast<int>(valueOf(toggle));
}

template <typename ButtonType> auto isPressed(ButtonType &button) -> bool {
  return valueOf(button) > 0.5F;
}

template <typename T> auto isHigh(T &input) -> bool {
  return voltageAt(input) > 1.F;
}

template <typename ItemType, int N, typename ToggleType = rack::engine::Param>
auto selected(ToggleType &toggle, std::array<ItemType, N> const &items)
    -> ItemType {
  return items[positionOf(toggle)];
}

template <int N, typename ToggleType = rack::engine::Param>
auto selectedRange(ToggleType &toggle,
                   std::array<Range const *, N> const &ranges)
    -> Range const * {
  return selected<Range const *, N>(toggle, ranges);
}

/**
 * Returns the taper selected by the given switch.
 */
template <typename ToggleType>
auto selectedTaper(ToggleType &toggle) -> taper::VariableTaper const * {
  return selected<taper::VariableTaper const *, 2>(toggle,
                                                   taper::variable_tapers);
}

template <typename KnobType, typename InputType>
auto rotation(KnobType &knob, InputType &cvInput) -> float {
  static constexpr auto cvModulationRatio = 0.1F;
  auto const rotation = rotationOf(knob);
  auto const cv = voltageAt(cvInput);
  auto const modulation = cv * cvModulationRatio;
  return rotation + modulation;
}

template <typename KnobType, typename InputType>
auto rotation(KnobType &knob, InputType &cvInput, KnobType &avKnob) -> float {
  static auto constexpr avModulationRange = Range{-0.1F, 0.1F};
  auto const rotation = rotationOf(knob);
  auto const cv = voltageAt(cvInput);
  auto const av = rotationOf(avKnob);
  auto const cvModulationRatio = avModulationRange.scale(av);
  auto const modulation = cv * cvModulationRatio;
  return rotation + modulation;
}

static inline auto taperedAndScaledRotation(float rotation,
                                            taper::FixedTaper const &taper,
                                            Range const &range) -> float {
  return range.scale(taper.apply(rotation));
}

template <typename KnobType>
auto taperedAndScaledRotation(KnobType &knob, taper::FixedTaper const &taper,
                              Range const &range) -> float {
  return taperedAndScaledRotation(rotationOf(knob), taper, range);
}

template <typename KnobType, typename InputType>
auto taperedAndScaledRotation(KnobType &knob, InputType &cvInput,
                              taper::FixedTaper const &taper,
                              Range const &range) -> float {
  return taperedAndScaledRotation(rotation(knob, cvInput), taper, range);
}

template <typename KnobType, typename InputType>
auto taperedAndScaledRotation(KnobType &knob, InputType &cvInput,
                              KnobType &avKnob, taper::FixedTaper const &taper,
                              Range const &range) -> float {
  return taperedAndScaledRotation(rotation(knob, cvInput, avKnob), taper,
                                  range);
}

} // namespace dhe
