#pragma once
#include "Inputs.h"
#include "modules/components/Taper.h"

#include <engine/Module.hpp>
#include <functional>

namespace dhe {
/**
 * Creates a function that applies a J taper with curvature controlled by a knob.
 */
auto jTaperFunction(rack::engine::Module const *module, int knobId) -> std::function<float(float)>;

/**
 * Creates a function that applies a taper with a curvature controlled by a modulated knob, and a shape selected by a
 * switch. The amount of modulation is determined by the voltage of a CV input.
 */
auto selectableShapeTaperFunction(rack::engine::Module const *module, int knobId, int cvId, int switchId)
    -> std::function<float(float)>;

/**
 * Creates a function that applies a taper with a curvature controlled by a modulated knob, and a shape selected by a
 * switch. The amount of modulation is determined by the voltage of a CV input, multiplied by the value of an
 * attenuverter.
 */
auto selectableShapeTaperFunction(rack::engine::Module const *module, int knobId, int cvId, int avId, int switchId)
    -> std::function<float(float)>;

/**
 * Returns the taper selected by the given switch.
 */
static inline auto selectedTaper(rack::engine::Module const *module, int switchId) -> taper::VariableTaper const * {
  return selected<taper::VariableTaper const *, 2>(module, switchId, taper::variableTapers);
}

} // namespace dhe
