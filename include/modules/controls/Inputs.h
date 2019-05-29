#pragma once

#include "modules/components/Taper.h"
#include "util/Range.h"

#include <engine/Module.hpp>

namespace dhe {

inline auto position(rack::engine::Module *module, int switchId) -> int {
  return static_cast<int>(module->params[switchId].getValue());
}

inline auto rotation(rack::engine::Module *module, int knobId, int cvId) -> float {
  static constexpr auto cvToModulation = 0.1F;
  auto const rotation = module->params[knobId].getValue();
  auto const cv = module->inputs[cvId].getVoltage();
  auto const modulation = cv * cvToModulation;
  return rotation + modulation;
}

inline auto rotation(rack::engine::Module *module, int knobId, int cvId, int avId) -> float {
  static constexpr auto avRange = Range{-0.1F, 0.1F};
  auto const rotation = module->params[knobId].getValue();
  auto const cv = module->inputs[cvId].getVoltage();
  auto const av = module->params[avId].getValue();
  return rotation + avRange.scale(av) * cv;
}

inline auto scaled(rack::engine::Module *module, int knobId, int cvId, Range const &range) -> float {
  return range.scale(rotation(module, knobId, cvId));
}

inline auto scaled(rack::engine::Module *module, int knobId, int cvId, int avId, Range const &range) -> float {
  return range.scale(rotation(module, knobId, cvId, avId));
}

template <typename T, int N>
auto selected(rack::engine::Module *module, int switchId, std::array<T, N> const &items) -> float {
  auto const selection = static_cast<int>(module->params[switchId].getValue());
  return items[selection];
}

inline auto taperedAndScaled(rack::engine::Module *module, int knobId, int cvId, int avId,
                             taper::FixedTaper const &taper, Range const &range) -> float {
  return range.scale(taper.apply(rotation(module, knobId, cvId, avId)));
}
} // namespace dhe