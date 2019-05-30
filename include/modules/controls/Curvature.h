#pragma once

#include "Controls.h"

#include <engine/Module.hpp>

namespace dhe {
namespace curvature {
  /**
   * Configures the param and display for a curvature knob.
   */
  void configKnob(rack::engine::Module *module, int knobId, std::string const &name = "Curvature",
                  float initialRotation = knob::centered);

  /**
   * Configures the param and display for a curve shape switch.
   */
  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Shape",
                    int initialState = 0);
} // namespace curvature

namespace taper {
  /**
   * Creates a function that applies a J taper with curvature controlled by a knob.
   */
  auto jShaped(rack::engine::Module *module, int knobId) -> std::function<float(float)>;

  /**
   * Creates a function that applies a taper with a curvature controlled by a modulated knob, and a shape selected by a
   * switch. The amount of modulation is determined by the voltage of a CV input.
   */
  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int switchId)
      -> std::function<float(float)>;

  /**
   * Creates a function that applies a taper with a curvature controlled by a modulated knob, and a shape selected by a
   * switch. The amount of modulation is determined by the voltage of a CV input, multiplied by the value of an
   * attenuverter.
   */
  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float(float)>;
} // namespace taper
} // namespace dhe
