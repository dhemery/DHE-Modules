#pragma once

#include "components/range.h"
#include "controls/common-inputs.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {
static auto constexpr centered_rotation = 0.5F;

/**
 * Configures the param and display for a gain knob.
 */
static inline void config_knob(rack::engine::Module *module, int knob_id,
                               std::string const &knob_name,
                               std::string const &units, Range range,
                               float initial_rotation = centered_rotation) {
  module->configParam(knob_id, 0.F, 1.F, initial_rotation, knob_name, units,
                      0.F, range.size(), range.lower_bound());
}

/**
 * Configures the param and display for a knob with a fixed, linear range,
 * displayed as a percentage.
 */
static inline void config_percentage_knob(rack::engine::Module *module,
                                          int knob_id,
                                          std::string const &knob_name,
                                          Range range = rotation_range) {
  module->configParam(knob_id, 0.F, 1.F, 0.5F, knob_name, "%", 0.F,
                      range.size() * 100.F, range.lower_bound() * 100.F);
}

/**
 * Configures the param and display for an attenuator knob.
 */
static inline void config_attenuator(rack::engine::Module *module, int knob_id,
                                     std::string const &knob_name) {
  config_percentage_knob(module, knob_id, knob_name, attenuator_range);
}

/**
 * Configures the param and display for an attenuverter knob.
 */
static inline void config_attenuverter(rack::engine::Module *module,
                                       int knob_id,
                                       std::string const &knob_name) {
  config_percentage_knob(module, knob_id, knob_name, attenuverter_range);
}

/**
 * Configures the param and display for a momentary button.
 */
static inline void config_gain(rack::engine::Module *module, int knob_id,
                               std::string const &knob_name) {
  config_percentage_knob(module, knob_id, knob_name, gain_range);
}

template <int N>
class FrameWidgetParamQuantity : public rack::engine::ParamQuantity {
public:
  void set_frame_names(std::array<char const *, N> const &names) {
    frame_names_ = names;
  }
  auto getDisplayValueString() -> std::string override {
    auto const frame = static_cast<int>(getValue());
    return frame_names_[frame];
  }

private:
  std::array<char const *, N> frame_names_;
};

template <int N>
void config_frame_widget_states(rack::engine::Module *module, int param_id,
                                std::string const &param_name,
                                std::array<char const *, N> const &state_names,
                                int initial_state) {
  module->configParam<FrameWidgetParamQuantity<N>>(
      param_id, 0.F, (float)(N - 1), (float)initial_state, param_name);
  auto const control_display = dynamic_cast<FrameWidgetParamQuantity<N> *>(
      module->paramQuantities[param_id]);
  control_display->set_frame_names(state_names);
}

/**
 * Configures the param and display for a momentary button.
 */
static inline void config_button(rack::engine::Module *module, int button_id,
                                 std::string const &button_name,
                                 std::array<char const *, 2> const &state_names,
                                 int initial_state) {
  config_frame_widget_states<2>(module, button_id, button_name, state_names,
                                initial_state);
}

/**
 * Configures the param and display for a toggle that represents a sequence of
 * states.
 */
template <int N>
void config_toggle(rack::engine::Module *module, int toggle_id,
                   std::string const &toggle_name,
                   std::array<char const *, N> const &state_names,
                   int initial_state) {
  config_frame_widget_states<N>(module, toggle_id, toggle_name, state_names,
                                initial_state);
}

} // namespace dhe
