#pragma once

#include "CommonConfig.h"
#include "components/Taper.h"
#include "controls/CommonInputs.h"
#include "controls/DurationInputs.h"

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
    auto const tapered = durationKnobTaper.apply(rotation);
    return range()->scale(tapered);
  }

  void setDisplayValue(float durationSeconds) override {
    auto const tapered = range()->normalize(durationSeconds);
    auto const rotation = durationKnobTaper.invert(tapered);
    setValue(rotation);
  }

  void setRangeSupplier(std::function<Range const *()> const &rangeSupplier) {
    this->range = rangeSupplier;
  }

private:
  std::function<Range const *()> range;
};

static inline void
configDurationKnob(rack::engine::Module *module, int knobId,
                   std::function<Range const *()> const &rangeSupplier,
                   std::string const &name, float initialPosition) {
  module->configParam<DurationKnobParamQuantity>(knobId, 0.F, 1.F,
                                                 initialPosition, name, " s");
  auto *knobParamQuantity = dynamic_cast<DurationKnobParamQuantity *>(
      module->paramQuantities[knobId]);
  knobParamQuantity->setRangeSupplier(rangeSupplier);
}

/**
 * Configures the param and display for a duration knob with a fixed range.
 */
static inline void
configDurationKnob(rack::engine::Module *module, int knobId, Range const &range,
                   std::string const &name = "Duration",
                   float initialRotation = centered_rotation) {
  auto const rangeSupplier = [range]() -> Range const * { return &range; };
  configDurationKnob(module, knobId, rangeSupplier, name, initialRotation);
}

/**
 * Configures the param and display for a duration knob with a range selected by
 * a switch.
 */
static inline void
configDurationKnob(rack::engine::Module *module, int knobId, int switchId,
                   std::string const &name = "Duration",
                   float initialRotation = centered_rotation) {
  auto const rangeSupplier = [module, switchId]() -> Range const * {
    return selectedRange<3>(module->params[switchId], durationRanges);
  };
  configDurationKnob(module, knobId, rangeSupplier, name, initialRotation);
}

/**
 * Configures the param and display for a duration range switch.
 */
static inline void
configDurationRangeSwitch(rack::engine::Module *module, int switchId,
                          std::string const &name = "Duration Range",
                          int initialPosition = 1) {
  static auto const positionNames =
      std::array<std::string, 3>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};
  config_toggle<3>(module, switchId, name, positionNames, initialPosition);
}
} // namespace dhe
