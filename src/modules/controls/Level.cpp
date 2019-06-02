#include "modules/controls/Level.h"

#include "modules/controls/Controls.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {

namespace level {
  auto const bipolarRange = Range{-5.F, 5.F};
  auto const unipolarRange = Range{0.F, 10.F};

  const std::array<Range const *, 2> ranges{&bipolarRange, &unipolarRange};

  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int switchId) -> std::function<float()> {
    return scaledRotationFunction<2>(module, knobId, cvId, switchId, level::ranges);
  }

  auto withUnipolarRange(rack::engine::Module *module, int knobId) -> std::function<float()> {
    return scaledRotationFunction(module, knobId, unipolarRange);
  }

  class KnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override {
      auto const rotation = getValue();
      return range()->scale(rotation);
    }

    void setDisplayValue(float displayValue) override {
      auto const normalized = range()->normalize(displayValue);
      setValue(normalized);
    }

    void setRangeSupplier(std::function<Range const *()> const &supplier) { this->range = supplier; }

  private:
    std::function<Range const *()> range;
  };

  void configKnob(rack::engine::Module *module, int knobId, std::function<Range const *()> const &rangeSupplier,
                  std::string const &name, float initialPosition) {
    module->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, initialPosition, name, " V");
    auto knobParamQuantity = dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
    knobParamQuantity->setRangeSupplier(rangeSupplier);
  }

  void configKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name,
                  float initialRotation) {
    auto getRange = rangeSelectorFunction<2>(module, switchId, level::ranges);
    configKnob(module, knobId, getRange, name, initialRotation);
  }

  void configKnob(rack::engine::Module *module, int knobId, Range const &range, std::string const &name,
                  float initialRotation) {
    auto getRange = [range]() -> Range const * { return &range; };
    configKnob(module, knobId, getRange, name, initialRotation);
  }

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name, int initialState) {
    static auto const stateNames = std::array<std::string, 2>{"±5 V", "0–10 V"};
    configToggle<2>(module, switchId, name, stateNames, initialState);
  }
} // namespace level
} // namespace dhe
