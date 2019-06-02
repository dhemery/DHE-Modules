#pragma once

#include "modules/components/Taper.h"
#include "util/Range.h"

#include <engine/Module.hpp>

namespace dhe {

static auto constexpr rotationRange = Range{0.F, 1.F};
static auto constexpr centeredRotation = 0.5F;

static inline auto paramValue(rack::engine::Module const *module, int paramId) -> float {
  return const_cast<rack::engine::Module *>(module)->params[paramId].getValue();
}

static inline auto inputVoltage(rack::engine::Module const *module, int inputId) -> float {
  return const_cast<rack::engine::Module *>(module)->inputs[inputId].getVoltage();
}

static inline auto switchPosition(rack::engine::Module const *module, int switchId) -> int {
  return static_cast<int>(paramValue(module, switchId));
}

template <typename T, int N>
auto selected(rack::engine::Module const *module, int switchId, std::array<T, N> const &items) -> T {
  return items[switchPosition(module, switchId)];
}

template <int N>
auto selectedRange(rack::engine::Module const *module, int switchId, std::array<Range const *, N> const &ranges)
    -> Range const * {
  return selected<Range const *, N>(module, switchId, ranges);
}

static inline auto buttonIsPressed(rack::engine::Module const *module, int buttonId) -> bool {
  return paramValue(module, buttonId) > 0.5F;
}

static inline auto inputIsConnected(rack::engine::Module const *module, int inputId) -> bool {
  return const_cast<rack::engine::Module *>(module)->inputs[inputId].isConnected();
}

static inline auto inputIsHigh(rack::engine::Module *module, int inputId) -> bool {
  return module->inputs[inputId].getVoltage() > 1.F;
}

static inline auto inputIsHighOrButtonIsPressed(rack::engine::Module *module, int inputId, int buttonId) -> bool {
  return module->inputs[inputId].getVoltage() > 1.F || module->params[buttonId].getValue() > 0.5F;
}

static inline auto rotation(rack::engine::Module const *module, int knobId) -> float {
  return paramValue(module, knobId);
}

static inline auto rotation(rack::engine::Module const *module, int knobId, int cvId) -> float {
  static constexpr auto cvModulationRatio = 0.1F;
  auto const rotation = paramValue(module, knobId);
  auto const cv = inputVoltage(module, cvId);
  auto const modulation = cv * cvModulationRatio;
  return rotation + modulation;
}

static inline auto rotation(rack::engine::Module const *module, int knobId, int cvId, int avId) -> float {
  static auto constexpr avModulationRange = Range{-0.1F, 0.1F};
  auto const rotation = paramValue(module, knobId);
  auto const cv = inputVoltage(module, cvId);
  auto const av = paramValue(module, avId);
  auto const cvModulationRatio = avModulationRange.scale(av);
  auto const modulation = cv * cvModulationRatio;
  return rotation + modulation;
}

static inline auto scaledRotation(rack::engine::Module *module, int knobId, Range const &range) -> float {
  return range.scale(rotation(module, knobId));
}

static inline auto scaledRotation(rack::engine::Module const *module, int knobId, int cvId, Range const &range)
    -> float {
  return range.scale(rotation(module, knobId, cvId));
}

static inline auto scaledRotation(rack::engine::Module *module, int knobId, int cvId, int avId, Range const &range)
    -> float {
  return range.scale(rotation(module, knobId, cvId, avId));
}

template <int N>
auto scaledRotation(rack::engine::Module *module, int knobId, int cvId, int rangeSwitchId,
                    std::array<Range const *, N> ranges) -> float {
  auto const range = selected<Range const *, N>(module, rangeSwitchId, ranges);
  return range->scale(rotation(module, knobId, cvId));
}

template <int N>
auto scaledRotation(rack::engine::Module *module, int knobId, int cvId, int avId, int rangeSwitchId,
                    std::array<Range const *, N> ranges) -> float {
  auto const range = selectedRange<N>(module, rangeSwitchId, ranges);
  return range->scale(rotation(module, knobId, cvId, avId));
}

static inline auto taperedAndScaledRotation(rack::engine::Module *module, int knobId, taper::FixedTaper const &taper,
                                            Range const &range) -> float {
  return range.scale(taper.apply(rotation(module, knobId)));
}

static inline auto taperedAndScaledRotation(rack::engine::Module const *module, int knobId, int cvId, int avId,
                                            taper::FixedTaper const &taper, Range const &range) -> float {
  return range.scale(taper.apply(rotation(module, knobId, cvId, avId)));
}

template <int N>
static inline auto taperedAndScaledRotation(rack::engine::Module *module, int knobId, int cvId,
                                            taper::FixedTaper const &taper, int switchId,
                                            std::array<Range const *, N> const &ranges) -> float {
  auto const range = selectedRange<N>(module, switchId, ranges);
  return range->scale(taper.apply(rotation(module, knobId, cvId)));
}
} // namespace dhe
