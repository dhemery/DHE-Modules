#include "DurationConfig.h"

#include "CommonInputs.h"
#include "DurationInputs.h"
#include "Taper.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>
#include <functional>

namespace dhe {

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

  void setRangeSupplier(std::function<Range const *()> const &rangeSupplier) { this->range = rangeSupplier; }

private:
  std::function<Range const *()> range;
};

void configDurationKnob(rack::engine::Module *module, int knobId, std::function<Range const *()> const &rangeSupplier,
                        std::string const &name, float initialPosition) {
  module->configParam<DurationKnobParamQuantity>(knobId, 0.F, 1.F, initialPosition, name, " s");
  auto knobParamQuantity = dynamic_cast<DurationKnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->setRangeSupplier(rangeSupplier);
}

void configDurationKnob(rack::engine::Module *module, int knobId, Range const &range, std::string const &name,
                        float initialRotation) {
  auto const rangeSupplier = [range]() -> Range const * { return &range; };
  configDurationKnob(module, knobId, rangeSupplier, name, initialRotation);
}

void configDurationKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name,
                        float initialRotation) {
  auto const rangeSupplier
      = [module, switchId]() -> Range const * { return selectedRange<3>(module->params[switchId], durationRanges); };
  configDurationKnob(module, knobId, rangeSupplier, name, initialRotation);
}

void configDurationRangeSwitch(rack::engine::Module *module, int switchId, std::string const &name,
                               int initialPosition) {
  static auto const positionNames = std::array<std::string, 3>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};
  configToggle<3>(module, switchId, name, positionNames, initialPosition);
}
} // namespace dhe
