#pragma once

#include <functional>
#include <string>

#include <engine/Module.hpp>

#include "util/range.h"

namespace dhe {

namespace level {
void configKnob(rack::engine::Module *module, int knobId, Range const &range,
                std::string const &name = "Level", float initialPosition = 0.5);

void configKnob(rack::engine::Module *module, int knobId, int switchId,
                std::string const &name = "Level", float initialPosition = 0.5);

void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name = "Level Range",
                  int initialPosition = 1);
} // namespace level

} // namespace dhe
