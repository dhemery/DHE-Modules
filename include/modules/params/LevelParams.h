#pragma once

#include <functional>

#include <engine/Module.hpp>

#include "util/range.h"

namespace dhe {

namespace level {
void configKnob(rack::engine::Module *module, int knobId, Range const &range);

void configKnob(rack::engine::Module *module, int knobId, int switchId);

void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &range);

void configSwitch(rack::engine::Module *module, int switchId,
                  int initialPosition = 1);
} // namespace level

} // namespace dhe
