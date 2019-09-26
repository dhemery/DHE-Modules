#pragma once

#include "modules/components/Range.h"
#include "modules/components/Taper.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>

namespace dhe {

static auto constexpr attenuatorRange = Range{0.F, 1.F};
static auto constexpr attenuverterRange = Range{-1.F, 1.F};
static auto constexpr gainRange = Range{0.F, 2.F};

static auto constexpr rotationRange = Range{0.F, 1.F};

static auto constexpr bipolarSignalRange = Range{-5.F, 5.F};
static auto constexpr unipolarSignalRange = Range{0.F, 10.F};

extern std::array<Range const *, 2> const signalRanges;

static inline auto paramValue(rack::engine::Param const &param) -> float {
  return const_cast<rack::engine::Param &>(param).getValue();
}

static inline auto inputVoltage(rack::engine::Input const &input) -> float {
  return const_cast<rack::engine::Input &>(input).getVoltage();
}

static inline auto switchPosition(rack::engine::Param const &toggle) -> int {
  return static_cast<int>(paramValue(toggle));
}

static inline auto buttonIsPressed(rack::engine::Param const &button) -> bool { return paramValue(button) > 0.5F; }

static inline auto inputIsHigh(rack::engine::Input const &input) -> bool { return inputVoltage(input) > 1.F; }

template <typename T, int N> auto selected(rack::engine::Param const &toggle, std::array<T, N> const &items) -> T {
  return items[switchPosition(toggle)];
}

template <int N>
auto selectedRange(rack::engine::Param const &toggle, std::array<Range const *, N> const &ranges) -> Range const * {
  return selected<Range const *, N>(toggle, ranges);
}

/**
 * Returns the taper selected by the given switch.
 */
static inline auto selectedTaper(rack::engine::Param const &toggle) -> taper::VariableTaper const * {
  return selected<taper::VariableTaper const *, 2>(toggle, taper::variableTapers);
}

static inline auto rotation(rack::engine::Param const &knob, rack::engine::Input const &cvInput) -> float {
  static constexpr auto cvModulationRatio = 0.1F;
  auto const rotation = paramValue(knob);
  auto const cv = inputVoltage(cvInput);
  auto const modulation = cv * cvModulationRatio;
  return rotation + modulation;
}

static inline auto rotation(rack::engine::Param const &knob, rack::engine::Input const &cvInput,
                            rack::engine::Param const &avKnob) -> float {
  static auto constexpr avModulationRange = Range{-0.1F, 0.1F};
  auto const rotation = paramValue(knob);
  auto const cv = inputVoltage(cvInput);
  auto const av = paramValue(avKnob);
  auto const cvModulationRatio = avModulationRange.scale(av);
  auto const modulation = cv * cvModulationRatio;
  return rotation + modulation;
}

static inline auto taperedAndScaledRotation(rack::engine::Param const &knob, taper::FixedTaper const &taper,
                                            Range const &range) -> float {
  return range.scale(taper.apply(paramValue(knob)));
}

static inline auto taperedAndScaledRotation(rack::engine::Param const &knob, rack::engine::Input const &cvInput,
                                            taper::FixedTaper const &taper, Range const &range) -> float {
  return range.scale(taper.apply(rotation(knob, cvInput)));
}

static inline auto taperedAndScaledRotation(rack::engine::Param const &knob, rack::engine::Input const &cvInput,
                                            rack::engine::Param const &avKnob, taper::FixedTaper const &taper,
                                            Range const &range) -> float {
  return range.scale(taper.apply(rotation(knob, cvInput, avKnob)));
}

} // namespace dhe
