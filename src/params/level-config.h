#pragma once

#include "controls/switches.h"

#include "components/range.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

#include <functional>
#include <string>

namespace dhe {
class Range;

class LevelKnobParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValue() -> float override {
    auto const rotation = getValue();
    return range_().scale(rotation);
  }

  void setDisplayValue(float display_value) override {
    auto const normalized = range_().normalize(display_value);
    setValue(normalized);
  }

  void set_range_supplier(std::function<Range()> const &supplier) {
    this->range_ = supplier;
  }

private:
  std::function<Range()> range_;
};

static inline void
config_level_knob(rack::engine::Module *module, int knob_id,
                  std::function<Range()> const &range_supplier,
                  std::string const &name, float initial_position) {
  module->configParam<LevelKnobParamQuantity>(knob_id, 0.F, 1.F,
                                              initial_position, name, " V");
  auto *knob_param_quantity =
      dynamic_cast<LevelKnobParamQuantity *>(module->paramQuantities[knob_id]);
  knob_param_quantity->set_range_supplier(range_supplier);
}

/**
 * Configures the param and display for a level knob with a range selected by a
 * switch.
 */
static inline void config_level_knob(rack::engine::Module *module, int knob_id,
                                     int switch_id,
                                     std::string const &name = "Level",
                                     float initial_rotation = 0.5F) {
  auto const range_supplier = [module, switch_id]() -> Range {
    return selected_range<2>(module->params[switch_id], signal_ranges);
  };
  config_level_knob(module, knob_id, range_supplier, name, initial_rotation);
}
} // namespace dhe
