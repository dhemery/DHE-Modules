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

namespace attenuator {
  void config(rack::engine::Module *module, int knobId, std::string const &knobName) {
    knob::configPercentage(module, knobId, knobName, range);
  }
} // namespace attenuator

namespace attenuverter {
  void config(rack::engine::Module *module, int knobId, std::string const &knobName) {
    knob::configPercentage(module, knobId, knobName, range);
  }
} // namespace attenuverter

namespace button {
  void config(rack::engine::Module *module, int buttonId, std::string const &buttonName,
              std::array<std::string, 2> const &stateNames, int initialState) {
    configFrameWidgetControl<2>(module, buttonId, buttonName, stateNames, initialState);
  }
} // namespace button

namespace gain {
  void config(rack::engine::Module *module, int knobId, std::string const &knobName) {
    knob::configPercentage(module, knobId, knobName, range);
  }
} // namespace gain

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

  void config(rack::engine::Module *module, int knobId, std::string const &knobName, std::string const &units,
              Range const &range, float initialRotation) {
    module->configParam(knobId, 0.F, 1.F, initialRotation, knobName, units, 0.F, range.size(), range.lowerBound);
  }

  void configPercentage(rack::engine::Module *module, int knobId, std::string const &knobName, Range const &range) {
    module->configParam(knobId, 0.F, 1.F, 0.5F, knobName, "%", 0.F, range.size() * 100.F, range.lowerBound * 100.F);
  }

  auto taperedAndScaled(std::function<float()> const &rotation, const taper::FixedTaper &taper, Range const &range)
      -> std::function<float()> {
    return [rotation, range, &taper]() {
      auto const tapered = taper.apply(rotation());
      auto const scaled = range.scale(tapered);
      return scaled;
    };
  }

  auto taperedAndScaled(std::function<float()> const &rotation, taper::FixedTaper const &taper,
                        std::function<Range const *()> const &range) -> std::function<float()> {
    return [rotation, range, &taper]() {
      auto const tapered = taper.apply(rotation());
      auto const scaled = range()->scale(tapered);
      return scaled;
    };
  }
} // namespace knob

namespace range {
  auto scaleTo(std::function<Range const *()> const &range) -> std::function<float(float)> {
    return [range](float proportion) -> float { return range()->scale(proportion); };
  }

  auto scaleTo(Range const &range) -> std::function<float(float)> {
    return [range](float proportion) -> float { return range.scale(proportion); };
  }

  template <int N>
  auto selector(rack::engine::Module *module, int switchId, std::array<Range const *, N> const &ranges)
      -> std::function<Range const *()> {
    auto switchParam = &module->params[switchId];
    return [switchParam, ranges]() -> Range const * {
      auto const selection = static_cast<int>(switchParam->getValue());
      return ranges[selection];
    };
  }

  template auto selector<2>(rack::engine::Module *module, int switchId, std::array<Range const *, 2> const &ranges)
      -> std::function<Range const *()>;

  template auto selector<3>(rack::engine::Module *module, int switchId, std::array<Range const *, 3> const &ranges)
      -> std::function<Range const *()>;

} // namespace range

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
  template void config<4>(rack::engine::Module *module, int toggleId, std::string const &toggleName,
                          std::array<std::string, 4> const &stateNames, int initialState);
} // namespace toggle

} // namespace dhe
