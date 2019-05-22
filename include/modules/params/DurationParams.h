#pragma once

#include <functional>

#include <engine/Module.hpp>

#include "util/range.h"

namespace DHE {

namespace Duration {
void configKnob(rack::engine::Module *module, int knobId, Range const &);

void configKnob(rack::engine::Module *module, int knobId, int switchId);

void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &getRange);

void configSwitch(rack::engine::Module *module, int switchId);
} // namespace Duration

} // namespace DHE
