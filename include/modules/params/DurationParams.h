#pragma once

#include <functional>
#include <string>

#include <engine/Module.hpp>

#include "util/range.h"

namespace dhe {

namespace duration {
void configKnob(rack::engine::Module *module, int knobId, Range const &,
                std::string const &name = "Duration",
                float initialPosition = 0.5);

void configKnob(rack::engine::Module *module, int knobId, int switchId,
                std::string const &name = "Duration",
                float initialPosition = 0.5);

void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name = "Duration Range",
                  int initialPosition = 1);
} // namespace duration

} // namespace dhe
