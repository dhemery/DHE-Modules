#include "modules/controls/Functions.h"

namespace dhe {

auto buttonIsPressedFunction(rack::engine::Module const *module, int buttonId) -> std::function<bool()> {
  return [module, buttonId]() -> bool { return buttonIsPressed(module, buttonId); };
}

auto inputIsConnectedFunction(rack::engine::Module const *module, int inputId) -> std::function<bool()> {
  return [module, inputId]() -> bool { return inputIsConnected(module, inputId); };
}

auto inputIsHighFunction(rack::engine::Module const *module, int inputId) -> std::function<bool()> {
  return [module, inputId]() -> bool { return inputIsHigh(module, inputId); };
}

auto inputIsHighOrButtonIsPressedFunction(rack::engine::Module const *module, int inputId, int buttonId)
    -> std::function<bool()> {
  return [module, inputId, buttonId]() -> bool {
    return inputIsHigh(module, inputId) || buttonIsPressed(module, buttonId);
  };
}

auto rotationFunction(rack::engine::Module const *module, int knobId, int cvId, int avId) -> std::function<float()> {
  return [module, knobId, cvId, avId]() -> float { return rotation(module, knobId, cvId, avId); };
}

auto scaledRotationFunction(rack::engine::Module const *module, int knobId, Range const &range)
    -> std::function<float()> {
  return [module, knobId, range]() -> float { return scaledRotation(module, knobId, range); };
}

auto taperedAndScaledRotationFunction(rack::engine::Module const *module, int knobId, taper::FixedTaper const &taper,
                                      Range const &range) -> std::function<float()> {
  return [module, knobId, &taper, &range]() -> float { return taperedAndScaledRotation(module, knobId, taper, range); };
}

auto taperedAndScaledRotationFunction(rack::engine::Module const *module, int knobId, int cvId, int avId,
                                      taper::FixedTaper const &taper, Range const &range) -> std::function<float()> {
  return [module, knobId, cvId, avId, &taper, &range]() -> float {
    return taperedAndScaledRotation(module, knobId, cvId, avId, taper, range);
  };
}
} // namespace dhe
