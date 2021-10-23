#pragma once

#include "components/range.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

#include <string>

namespace dhe {
static auto constexpr preset_version_key = "preset_version";
static auto constexpr centered_rotation = 0.5F;

static inline void config_switch(rack::engine::Module *module, int id,
                                 std::string const &name,
                                 std::vector<std::string> const &labels,
                                 int initial_state = 0) {
  auto const max_value = static_cast<float>(labels.size() - 1);
  auto const default_value = static_cast<float>(initial_state);
  module->configSwitch(id, 0.F, max_value, default_value, name, labels);
}

/**
 * Configures the param and display for a momentary button.
 */
static inline void config_button(rack::engine::Module *module, int id,
                                 std::string const &name,
                                 int initial_state = 0) {
  auto const default_value = static_cast<float>(initial_state);
  module->configSwitch(id, 0.F, 1.F, default_value, name);
}

} // namespace dhe
