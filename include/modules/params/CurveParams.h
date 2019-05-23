#pragma once

#include <string>

#include <engine/Module.hpp>

namespace dhe {
namespace curvature {
void configKnob(rack::engine::Module *module, int knobId,
                std::string const &name = "Curvature",
                float initialPosition = 0.5f);
void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name = "Shape", int initialPosition = 0);

}; // namespace curvature
} // namespace dhe