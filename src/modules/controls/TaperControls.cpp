#include "modules/controls/TaperControls.h"

#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"
#include "modules/controls/CurvatureControls.h"
#include "modules/controls/Inputs.h"
#include "util/Sigmoid.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {
auto jTaperFunction(rack::engine::Module const *module, int knobId) -> std::function<float(float)> {
  return [module, knobId](float input) -> float {
    auto const curvature = dhe::curvature(module, knobId);
    return taper::variableJTaper.apply(input, curvature);
  };
}

auto selectableShapeTaperFunction(rack::engine::Module const *module, int knobId, int cvId, int switchId)
    -> std::function<float(float)> {
  return [module, knobId, cvId, switchId](float input) -> float {
    auto const curvature = dhe::curvature(module, knobId, cvId);
    auto const taper = selectedTaper(module, switchId);
    return taper->apply(input, curvature);
  };
}

auto selectableShapeTaperFunction(rack::engine::Module const *module, int knobId, int cvId, int avId, int switchId)
    -> std::function<float(float)> {
  return [module, knobId, cvId, avId, switchId](float input) -> float {
    auto const curvature = dhe::curvature(module, knobId, cvId, avId);
    auto const taper = selectedTaper(module, switchId);
    return taper->apply(input, curvature);
  };
}

} // namespace dhe
