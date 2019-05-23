#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

namespace level {

const std::array<Range const *, 2> ranges{&bipolarRange, &unipolarRange};

auto toSelectedRange(rack::engine::Module *module, int switchId)
    -> std::function<float(float)> {
  using namespace control;
  auto const selectedRange =
      range::selection<2>(module, switchId, level::ranges);
  return control::scale::toRange(selectedRange);
}

auto range(float switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

auto withFixedRange(rack::engine::Module *module, int knobId,
                    Range const &range) -> std::function<float()> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId);
  return knob::scaled(rotation, scale::toRange(range));
}

auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int switchId) -> std::function<float()> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId, cvId);
  return knob::scaled(rotation, toSelectedRange(module, switchId));
}

auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId,
                         int avId, int switchId) -> std::function<float()> {
  using namespace control;
  auto const rotation = knob::rotation(module, knobId, cvId, avId);
  return knob::scaled(rotation, toSelectedRange(module, switchId));
}

class RangeSwitchParamQuantity : public rack::engine::ParamQuantity {
public:
  auto getDisplayValueString() -> std::string override {
    return getValue() < 0.5 ? "±5 V" : "0–10 V";
  }
};

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

  std::function<Range const *()> range;
};

void configKnob(rack::engine::Module *module, int knobId,
                std::function<Range const *()> const &range,
                std::string const &name, float initialPosition) {
  module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, initialPosition,
                                         name, " V");
  auto knobParamQuantity =
      dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
  knobParamQuantity->range = range;
}

void configKnob(rack::engine::Module *module, int knobId, int switchId,
                std::string const &name, float initialPosition) {
  auto getRange = control::range::selection<2>(module, switchId, level::ranges);
  configKnob(module, knobId, getRange, name, initialPosition);
}

void configKnob(rack::engine::Module *module, int knobId, Range const &range,
                std::string const &name, float initialPosition) {
  auto getRange = [range]() -> Range const * { return &range; };
  configKnob(module, knobId, getRange, name, initialPosition);
}

void configSwitch(rack::engine::Module *module, int switchId,
                  std::string const &name, int initialPosition) {
  module->configParam<RangeSwitchParamQuantity>(switchId, 0.f, 1.f,
                                                initialPosition, name);
}
} // namespace level
} // namespace dhe
