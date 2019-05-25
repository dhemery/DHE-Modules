#include "modules/controls/Controls.h"

#include "util/Range.h"

namespace dhe {

namespace control {
  namespace knob {
    auto rotation(rack::engine::Module *module, int knobId) -> std::function<float()> {
      auto knobParam = &module->params[knobId];
      return [knobParam]() -> float { return knobParam->getValue(); };
    }

    auto rotation(rack::engine::Module *module, int knobId, int cvId) -> std::function<float()> {
      static auto constexpr attenuation = 0.1F;
      auto knobParam = &module->params[knobId];
      auto cvInput = &module->inputs[cvId];

      return [knobParam, cvInput]() -> float {
        auto const rotation = knobParam->getValue();
        auto const controlVoltage = cvInput->getVoltage();
        auto const modulation = controlVoltage * attenuation;
        return rotation + modulation;
      };
    }

    auto rotation(rack::engine::Module *module, int knobId, int cvId, int avId) -> std::function<float()> {
      static auto constexpr attenuationRange = Range{-0.1F, 0.1F};
      auto knobParam = &module->params[knobId];
      auto cvInput = &module->inputs[cvId];
      auto avParam = &module->params[avId];

      return [knobParam, cvInput, avParam]() -> float {
        auto const rotation = knobParam->getValue();
        auto const controlVoltage = cvInput->getVoltage();
        auto const attenuverterRotation = avParam->getValue();
        auto const attenuation = attenuationRange.scale(attenuverterRotation);
        auto const modulation = controlVoltage * attenuation;

        return rotation + modulation;
      };
    }

    auto scaled(std::function<float()> const &rotation, std::function<float(float)> const &scale)
        -> std::function<float()> {
      return [rotation, scale]() -> float { return scale(rotation()); };
    }

    auto scaled(std::function<float()> const &rotation, std::function<float(float)> const &taper,
                std::function<float(float)> const &scale) -> std::function<float()> {
      return [rotation, taper, scale]() -> float { return scale(taper(rotation())); };
    }
  } // namespace knob

  namespace scale {
    auto toRange(std::function<Range const *()> const &range) -> std::function<float(float)> {
      return [range](float proportion) -> float { return range()->scale(proportion); };
    }
    auto toRange(Range const &range) -> std::function<float(float)> {
      return [range](float proportion) -> float { return range.scale(proportion); };
    }

  } // namespace scale

} // namespace control
} // namespace dhe
