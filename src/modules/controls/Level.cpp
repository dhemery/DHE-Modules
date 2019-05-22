#include <engine/Module.hpp>

#include "modules/controls/Level.h"

namespace dhe {

namespace level {

const std::array<Range const *, 2> ranges{&bipolarRange, &unipolarRange};

auto range(float switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

auto withCvAndSwitch(rack::engine::Module *module, int knobId, int cvId,
                     int switchId) -> std::function<float()> {
  auto knobParam = &module->params[knobId];
  auto cvInput = &module->inputs[cvId];
  auto switchParam = &module->params[switchId];
  return [knobParam, cvInput, switchParam]() -> float {
    auto const rotation = knobParam->getValue();
    auto const controlVoltage = cvInput->getVoltage();
    auto const range = level::range(switchParam->getValue());
    auto const modulated = rotation + controlVoltage * 0.1f;
    return range->scale(modulated);
  };
}

auto withRange(rack::engine::Module *module, int knobId, Range const &range)
    -> std::function<float()> {
  auto knobParam = &module->params[knobId];
  return [knobParam, range]() -> float {
    auto const rotation = knobParam->getValue();
    return range.scale(rotation);
  };
}
} // namespace level
} // namespace dhe
