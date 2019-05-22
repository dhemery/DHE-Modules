#include <engine/Module.hpp>

#include "modules/controls/Duration.h"
#include "util/sigmoid.h"

namespace dhe {

namespace duration {

const float knobTaperCurvature = 0.8018017;

const std::array<Range const *, 3> ranges{&shortRange, &mediumRange,
                                          &longRange};

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
    auto const range = duration::range(switchParam->getValue());
    auto const modulated = rotation + controlVoltage * 0.1f;
    auto const tapered = Sigmoid::j_shape.taper(modulated, knobTaperCurvature);
    return range->scale(tapered);
  };
}

auto withRange(rack::engine::Module *module, int knobId, Range const &range)
    -> std::function<float()> {
  auto knobParam = &module->params[knobId];
  return [knobParam, range]() -> float {
    auto const rotation = knobParam->getValue();
    auto const tapered = Sigmoid::j_shape.taper(rotation, knobTaperCurvature);
    return range.scale(tapered);
  };
}
} // namespace duration
} // namespace dhe
