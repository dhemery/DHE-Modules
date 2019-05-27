#pragma once

#include "Controls.h"

#include <engine/Module.hpp>

namespace dhe {
namespace curvature {
  void configKnob(rack::engine::Module *module, int knobId, std::string const &name = "Curvature",
                  float initialRotation = knob::centered);

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Shape",
                    int initialState = 0);
} // namespace curvature

namespace taper {
  extern std::array<taper::VariableTaper const *, 2> const variableTapers;

  auto jShaped(rack::engine::Module *module, int knobId) -> std::function<float(float)>;

  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int switchId)
      -> std::function<float(float)>;

  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float(float)>;
} // namespace taper
} // namespace dhe
