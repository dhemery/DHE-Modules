#include "modules/controls/Curvature.h"

#include "modules/controls/Controls.h"
#include "util/Sigmoid.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
namespace curvature {
  /**
   * This curvature creates a gentle inverted S taper, increasing sensitivity in the middle of the input range and
   * decreasing sensitivity toward the extremes.
   */
  static auto constexpr knobTaperCurvature = -0.65F;
  static auto constexpr sigmoidRange = Range{-1.F, 1.F};

  inline auto toCurvature(float rotation) -> float {
    auto const scaled = sigmoidRange.scale(rotation);
    return sigmoid::curve(scaled, knobTaperCurvature);
  }

  inline auto toRotation(float curvature) -> float {
    auto const linearized = sigmoid::curve(curvature, -knobTaperCurvature);
    return sigmoidRange.normalize(linearized);
  }

  class KnobParamQuantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override { return toCurvature(getValue()); }

    void setDisplayValue(float curvature) override { setValue(toRotation(curvature)); }
  }; // namespace curvature

  inline auto knob(std::function<float()> const &rotation) -> std::function<float()> {
    return [rotation]() -> float {
      auto const clampedRotation = knob::rotationRange.clamp(rotation());
      return curvature::toCurvature(clampedRotation);
    };
  }

  void configKnob(rack::engine::Module *module, int knobId, std::string const &name, float initialRotation) {
    module->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, initialRotation, name);
  }

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name, int initialState) {
    static auto const stateNames = std::array<std::string, 2>{"J", "S"};
    toggle::config<2>(module, switchId, name, stateNames, initialState);
  }

} // namespace curvature

namespace taper {
  const std::array<taper::VariableTaper const *, 2> variableTapers{&variableJTaper, &variableSTaper};

  inline auto withSelectableShape(std::function<float()> const &curvature, rack::engine::Param *switchParam)
      -> std::function<float(float)> {
    return [curvature, switchParam](float input) -> float {
      auto const taperSelection = static_cast<int>(switchParam->getValue());
      auto const taper = variableTapers[taperSelection];
      return taper->apply(input, curvature());
    };
  }

  auto jShaped(rack::engine::Module *module, int knobId) -> std::function<float(float)> {
    auto const curvature = curvature::knob(knob::rotation(module, knobId));
    return [curvature](float input) -> float { return variableJTaper.apply(input, curvature()); };
  }

  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int switchId)
      -> std::function<float(float)> {
    auto const curvature = curvature::knob(knob::rotation(module, knobId, cvId));
    auto const switchParam = &module->params[switchId];
    return withSelectableShape(curvature, switchParam);
  }

  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float(float)> {
    auto const curvature = curvature::knob(knob::rotation(module, knobId, cvId, avId));
    auto const switchParam = &module->params[switchId];
    return withSelectableShape(curvature, switchParam);
  }
} // namespace taper
} // namespace dhe
