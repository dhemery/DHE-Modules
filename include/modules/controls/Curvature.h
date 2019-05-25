#pragma once

#include "util/Sigmoid.h"

#include <engine/Module.hpp>
#include <functional>

namespace dhe {
namespace curvature {

  float constexpr centeredRotation = 0.5f;

  /**
   * Creates a function that applies a taper with a selectable shape and
   * curvature.
   * @param knobId the ID of the knob that selects the curvature
   * @param cvId the ID of the control voltage input that modulates the knob
   * @param switchId the ID of the switch that selects the shape (J or S)
   */
  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int switchId)
      -> std::function<float(float)>;

  /**
   * Creates a function that applies a taper with a selectable shape and
   * curvature.
   * @param knobId the ID of the knob that selects the curvature
   * @param cvId the ID of the control voltage input that modulates the knob
   * @param avId the ID of the attenuverter param that amplifies the control
   * voltage
   * @param switchId the ID of the switch that selects the shape of the taper (J
   * or S)
   */
  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float(float)>;

  /**
   * Creates a function that applies a taper with a selectable curvature and
   * fixed shape (J or S).
   * @param knobId the ID of the knob that selects the curvature
   * @param shape the shape of the taper
   */
  auto withFixedShape(rack::engine::Module *module, int knobId, sigmoid::Shape const *shape)
      -> std::function<float(float)>;

  /**
   * Creates a function that maps a rotation to a curvature. The curvature is
   * useful to control the curve intensity of one of the sigmoid curve shapes.
   */
  auto rotationToCurvature() -> std::function<float(float)>;

  /**
   * Creates a function that maps a curvature to a rotation.
   */
  auto curvatureToRotation() -> std::function<float(float)>;

  /**
   * Creates a function that uses a switch to select a shape (J or S).
   * @param switchId the ID of the switch to select the shape
   */
  auto shape(rack::engine::Module *module, int switchId) -> std::function<sigmoid::Shape const *()>;

  void configKnob(rack::engine::Module *module, int knobId, std::string const &name = "Curvature",
                  float initialPosition = centeredRotation);

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Shape",
                    int initialPosition = 0);
} // namespace curvature

} // namespace dhe
