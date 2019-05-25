#include "modules/controls/Level.h"

#include "modules/controls/Controls.h"
#include "modules/controls/ToggleControls.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {

namespace level {

  const std::array<Range const *, 2> ranges{&bipolarRange, &unipolarRange};

  auto toSelectedRange(rack::engine::Module *module, int switchId) -> std::function<float(float)> {
    using namespace control;
    auto const selectedRange = range::selection<2>(module, switchId, level::ranges);
    return control::scale::toRange(selectedRange);
  }

  auto range(float switchPosition) -> Range const * { return ranges[static_cast<int>(switchPosition)]; }

  auto withFixedRange(rack::engine::Module *module, int knobId, Range const &range) -> std::function<float()> {
    using namespace control;
    auto const rotation = knob::rotation(module, knobId);
    return knob::scaled(rotation, scale::toRange(range));
  }

  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int switchId) -> std::function<float()> {
    using namespace control;
    auto const rotation = knob::rotation(module, knobId, cvId);
    return knob::scaled(rotation, toSelectedRange(module, switchId));
  }

  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int avId, int switchId)
      -> std::function<float()> {
    using namespace control;
    auto const rotation = knob::rotation(module, knobId, cvId, avId);
    return knob::scaled(rotation, toSelectedRange(module, switchId));
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
                  float initialPosition) {
    auto getRange = control::range::selection<2>(module, switchId, level::ranges);
    configKnob(module, knobId, getRange, name, initialPosition);
  }

  void configKnob(rack::engine::Module *module, int knobId, Range const &range, std::string const &name,
                  float initialPosition) {
    auto getRange = [range]() -> Range const * { return &range; };
    configKnob(module, knobId, getRange, name, initialPosition);
  }

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name, int initialPosition) {
    static auto const positionNames = std::array<std::string, 2>{"±5 V", "0–10 V"};
    toggle::config<2>(module, switchId, name, positionNames, initialPosition);
  }
} // namespace level
} // namespace dhe
