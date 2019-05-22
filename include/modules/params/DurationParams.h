#pragma once

#include <array>
#include <functional>

#include <engine/Module.hpp>

#include "util/range.h"

namespace DHE {

namespace Duration {
void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &getRange);

void configSwitch(rack::engine::Module *module, int switchId,
                  std::function<Range const *()> const &getRange);
} // namespace Duration

} // namespace DHE
