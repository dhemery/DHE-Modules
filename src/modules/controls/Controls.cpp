#include "modules/controls/Controls.h"

#include "util/Range.h"

namespace dhe {

template <int N> class FrameWidgetParamQuantity : public rack::engine::ParamQuantity {
  std::array<std::string, N> frameNames;

public:
  void setFrameNames(std::array<std::string, N> const &names) { frameNames = names; }
  auto getDisplayValueString() -> std::string override {
    auto const frame = static_cast<int>(getValue());
    return frameNames[frame];
  }
};

template <int N>
static void configFrameWidgetControl(rack::engine::Module *module, int paramId, std::string const &paramName,
                                     std::array<std::string, N> const &stateNames, int initialState) {
  module->configParam<FrameWidgetParamQuantity<N>>(paramId, 0.F, (float) (N - 1), (float) initialState, paramName);
  auto controlDisplay = dynamic_cast<FrameWidgetParamQuantity<N> *>(module->paramQuantities[paramId]);
  controlDisplay->setFrameNames(stateNames);
}

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

namespace button {
  void config(rack::engine::Module *module, int buttonId, std::string const &buttonName,
              std::array<std::string, 2> const &stateNames, int initialState) {
    configFrameWidgetControl<2>(module, buttonId, buttonName, stateNames, initialState);
  } // namespace button
} // namespace button

namespace toggle {
  template <int N>
  void config(rack::engine::Module *module, int toggleId, std::string const &toggleName,
              std::array<std::string, N> const &stateNames, int initialState) {
    configFrameWidgetControl<N>(module, toggleId, toggleName, stateNames, initialState);
  }

  template void config<2>(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                          std::array<std::string, 2> const &stateNames, int initialState);
  template void config<3>(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                          std::array<std::string, 3> const &stateNames, int initialState);
} // namespace toggle

} // namespace dhe
