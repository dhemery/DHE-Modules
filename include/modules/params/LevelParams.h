#pragma once

#include <functional>

#include <engine/Module.hpp>

#include "util/range.h"

namespace DHE {

namespace Level {
void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &getRange);

void configSwitch(rack::engine::Module *module, int switchId,
                  int initialPosition,
                  std::function<Range const *()> const &getRange);
} // namespace Level

} // namespace DHE
