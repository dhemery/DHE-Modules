#include "modules/controls/Controls.h"
#include "util/range.h"

namespace dhe {

namespace control {
namespace knob {
auto rotation(rack::engine::Module *module, int knobId)
    -> std::function<float()> {
  auto knobParam = &module->params[knobId];
  return [knobParam]() -> float { return knobParam->getValue(); };
}

auto rotation(rack::engine::Module *module, int knobId, int cvId)
    -> std::function<float()> {
  static auto constexpr cvToModulation = 0.1f;
  auto knobParam = &module->params[knobId];
  auto cvInput = &module->inputs[cvId];

  return [knobParam, cvInput]() -> float {
    auto const rotation = knobParam->getValue();
    auto const controlVoltage = cvInput->getVoltage();
    auto const modulation = controlVoltage * cvToModulation;
    return rotation + modulation;
  };
}

auto rotation(rack::engine::Module *module, int knobId, int cvId, int avId)
    -> std::function<float()> {
  static auto constexpr avRange = dhe::Range{-1.f, 1.f};
  auto knobParam = &module->params[knobId];
  auto cvInput = &module->inputs[cvId];
  auto avParam = &module->params[avId];

  return [knobParam, cvInput, avParam]() -> float {
    auto const rotation = knobParam->getValue();
    auto const controlVoltage = cvInput->getVoltage();
    auto const avMultiplier = avRange.scale(avParam->getValue());
    auto const modulation = controlVoltage * avMultiplier;
    return rotation + modulation;
  };
}

auto scaled(std::function<float()> const &rotation,
            std::function<float(float)> const &scale)
    -> std::function<float()> {
  return [rotation, scale]() -> float { return scale(rotation()); };
}

auto scaled(std::function<float()> const &rotation,
            std::function<float(float)> const &taper,
            std::function<float(float)> const &scale)
    -> std::function<float()> {
  return
      [rotation, taper, scale]() -> float { return scale(taper(rotation())); };
}
} // namespace knob

namespace scale {
auto toRange(std::function<const Range *()> range)
    -> std::function<float(float)> {
  return std::function<float(float)>();
}
auto toRange(const dhe::Range &) -> std::function<float(float)> {
  return std::function<float(float)>();
}

} // namespace scale
} // namespace control
} // namespace dhe
