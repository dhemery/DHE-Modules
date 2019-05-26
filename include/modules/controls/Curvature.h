#pragma once

#include "Controls.h"

#include <engine/Module.hpp>
#include <modules/components/Taper.h>

namespace dhe {
namespace curvature {
  enum Shape { J, S };

  void configKnob(rack::engine::Module *module, int knobId, std::string const &name = "Curvature",
                  float initialRotation = knob::centered);

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name = "Shape",
                    Shape initialShape = Shape::J);
} // namespace curvature

namespace taper {
  auto withSelectableShape(rack::engine::Module *module, int knobId, int cvId, int switchId)
      -> std::function<VariableTaper const *()>;
}
} // namespace dhe
