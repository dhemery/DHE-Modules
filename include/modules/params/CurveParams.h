#pragma once

#include <string>

#include <engine/Module.hpp>

namespace dhe {
namespace curvature {
void configKnob(rack::engine::Module *module, int knobId,
                std::string const &name = "Curvature", int initialPosition = 0);
void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name = "Shape", int initialPosition = 0);

}; // namespace curvature
} // namespace dhe