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

  static inline auto toCurvature(float rotation) -> float {
    auto const scaled = sigmoidRange.scale(rotation);
    return sigmoid::curve(scaled, knobTaperCurvature);
  }

  static inline auto toRotation(float curvature) -> float {
    auto const linearized = sigmoid::curve(curvature, -knobTaperCurvature);
    return sigmoidRange.normalize(linearized);
  }

  class KnobParamQuantity : public rack::engine::ParamQuantity {
    auto getDisplayValue() -> float override { return toCurvature(getValue()); }

    void setDisplayValue(float curvature) override { setValue(toRotation(curvature)); }
  }; // namespace curvature

  static inline auto knob(std::function<float()> const &rotation) -> std::function<float()> {
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
  static inline auto withSelectableShape(std::function<float()> const &curvature,
                                         std::function<VariableTaper const *()> const &taper)
      -> std::function<float(float)> {
    return [curvature, taper](float input) -> float { return taper()->apply(input, curvature()); };
  }

  auto jShaped(rack::engine::Module *module, int knobId) -> std::function<float(float)> {
    auto const curvature = curvature::knob(knob::rotation(module, knobId));
    return [curvature](float input) -> float { return variableJTaper.apply(input, curvature()); };
  }

  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int switchId)
      -> std::function<float(float)> {
    auto const curvature = curvature::knob(knob::rotation(module, knobId, cvId));
    auto const taper = selection::of<VariableTaper const *, 2>(module, switchId, variableTapers);
    return withSelectableShape(curvature, taper);
  }

  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float(float)> {
    auto const curvature = curvature::knob(knob::rotation(module, knobId, cvId, avId));
    auto const taper = selection::of<VariableTaper const *, 2>(module, switchId, variableTapers);
    return withSelectableShape(curvature, taper);
  }

} // namespace taper

} // namespace dhe
