#pragma once

#include "common-config.h"
#include "controls/common-inputs.h"
#include "controls/duration-inputs.h"

#include <array>
#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>
#include <functional>

namespace dhe {
class Range;
class DurationKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    auto const tapered = duration_knob_taper.apply(rotation);
    return range_().scale(tapered);
  }

  void setDisplayValue(float duration_seconds) override {
    auto const tapered = range_().normalize(duration_seconds);
    auto const rotation = duration_knob_taper.invert(tapered);
    setValue(rotation);
  }

  void set_range_supplier(std::function<Range()> const &range_supplier) {
    this->range_ = range_supplier;
  }

private:
  std::function<Range()> range_;
};

static inline void
config_duration_knob(rack::engine::Module *module, int knob_id,
                     std::function<Range()> const &range_supplier,
                     std::string const &name, float initial_position) {
  module->configParam<DurationKnobParamQuantity>(knob_id, 0.F, 1.F,
                                                 initial_position, name, " s");
  auto *knob_param_quantity = dynamic_cast<DurationKnobParamQuantity *>(
      module->paramQuantities[knob_id]);
  knob_param_quantity->set_range_supplier(range_supplier);
}

/**
 * Configures the param and display for a duration knob with a fixed range.
 */
static inline void
config_duration_knob(rack::engine::Module *module, int knob_id, Range range,
                     std::string const &name = "Duration",
                     float initial_rotation = centered_rotation) {
  auto const range_supplier = [range]() -> Range { return range; };
  config_duration_knob(module, knob_id, range_supplier, name, initial_rotation);
}

/**
 * Configures the param and display for a duration knob with a range selected by
 * a switch.
 */
static inline void
config_duration_knob(rack::engine::Module *module, int knob_id, int switch_id,
                     std::string const &name = "Duration",
                     float initial_rotation = centered_rotation) {
  auto const range_supplier = [module, switch_id]() -> Range {
    return selected_range<3>(module->params[switch_id], duration_ranges);
  };
  config_duration_knob(module, knob_id, range_supplier, name, initial_rotation);
}

static auto constexpr position_names =
    std::array<char const *, 3>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};

/**
 * Configures the param and display for a duration range switch.
 */
static inline void
config_duration_range_switch(rack::engine::Module *module, int switch_id,
                             std::string const &name = "Duration range",
                             int initial_position = 1) {
  config_toggle<3>(module, switch_id, name, position_names, initial_position);
}
} // namespace dhe
