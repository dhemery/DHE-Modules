#pragma once

#include "components/range.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

#include <string>

namespace dhe {
static auto constexpr preset_version_key = "preset_version";
static auto constexpr centered_rotation = 0.5F;

static inline void config_knob(rack::engine::Module *module, int knob_id,
                               std::string const &knob_name,
                               std::string const &units, Range range,
                               float initial_rotation = centered_rotation) {
  module->configParam(knob_id, 0.F, 1.F, initial_rotation, knob_name, units,
                      0.F, range.size(), range.lower_bound());
}

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

template <int N>
void config_toggle(rack::engine::Module *module, int toggle_id,
                   std::string const &toggle_name,
                   std::array<char const *, N> const &state_names,
                   int initial_state = 0) {
  auto labels =
      std::vector<std::string>{state_names.cbegin(), state_names.cend()};
  config_switch(module, toggle_id, toggle_name, labels, initial_state);
}

} // namespace dhe
