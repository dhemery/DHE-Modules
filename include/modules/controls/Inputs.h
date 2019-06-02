#pragma once

#include "modules/components/Taper.h"
#include "util/Range.h"

#include <engine/Module.hpp>

namespace dhe {

static auto constexpr rotationRange = Range{0.F, 1.F};
static auto constexpr centeredRotation = 0.5F;

template <typename T, int N>
auto selected(rack::engine::Module *module, int switchId, std::array<T, N> const &items) -> T {
  auto const selection = static_cast<int>(module->params[switchId].getValue());
  return items[selection];
}

template <int N>
auto selectedRange(rack::engine::Module *module, int switchId, std::array<Range const *, N> const &ranges)
    -> Range const * {
  return selected<Range const *, N>(module, switchId, ranges);
}

static inline auto isPressed(rack::engine::Module *module, int buttonId) -> bool {
  return module->params[buttonId].getValue() > 0.5F;
}

static inline auto position(rack::engine::Module *module, int switchId) -> int {
  return static_cast<int>(module->params[switchId].getValue());
}

static inline auto rotation(rack::engine::Module *module, int knobId) -> float {
  auto const rotation = module->params[knobId].getValue();
  return rotation;
}

static inline auto rotation(rack::engine::Module *module, int knobId, int cvId) -> float {
  static constexpr auto cvModulationRatio = 0.1F;
  auto const rotation = module->params[knobId].getValue();
  auto const cv = module->inputs[cvId].getVoltage();
  auto const modulation = cv * cvModulationRatio;
  return rotation + modulation;
}

static inline auto rotation(rack::engine::Module *module, int knobId, int cvId, int avId) -> float {
  static auto constexpr avModulationRange = Range{-0.1F, 0.1F};
  auto const rotation = module->params[knobId].getValue();
  auto const cv = module->inputs[cvId].getVoltage();
  auto const av = module->params[avId].getValue();
  auto const cvModulationRatio = avModulationRange.scale(av);
  auto const modulation = cv * cvModulationRatio;
  return rotation + modulation;
}

static inline auto scaledRotation(rack::engine::Module *module, int knobId, Range const &range) -> float {
  return range.scale(rotation(module, knobId));
}

static inline auto scaledRotation(rack::engine::Module *module, int knobId, int cvId, Range const &range) -> float {
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

static inline auto taperedAndScaledRotation(rack::engine::Module *module, int knobId, int cvId, int avId,
                                            taper::FixedTaper const &taper, Range const &range) -> float {
  return range.scale(taper.apply(rotation(module, knobId, cvId, avId)));
}

static inline auto taperedAndScaledRotation(rack::engine::Module *module, int knobId, int cvId,
                                            taper::FixedTaper const &taper, Range const &range) -> float {
  return range.scale(taper.apply(rotation(module, knobId, cvId)));
}

template <int N>
static inline auto taperedAndScaledRotation(rack::engine::Module *module, int knobId, taper::FixedTaper const &taper,
                                            int switchId, std::array<Range const *, N> const &ranges) -> float {
  auto const range = selectedRange<N>(module, switchId);
  return range->scale(taper.apply(rotation(module, knobId)));
}

template <int N>
static inline auto taperedAndScaledRotation(rack::engine::Module *module, int knobId, int cvId,
                                            taper::FixedTaper const &taper, int switchId,
                                            std::array<Range const *, N> const &ranges) -> float {
  auto const range = selectedRange<N>(module, switchId, ranges);
  return range->scale(taper.apply(rotation(module, knobId, cvId)));
}
} // namespace dhe
