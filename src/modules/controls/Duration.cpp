#include "modules/controls/Duration.h"

#include "modules/controls/Controls.h"
#include "util/sigmoid.h"

#include <engine/Module.hpp>
#include <engine/ParamQuantity.hpp>

namespace dhe {

namespace duration {

  /**
   * Each duration range is of the form [n, 1000n]. Given ranges of that form,
   * this curvature tapers the rotation so a knob positioned dead center
   * yields a duration equal to 1/10 of the range's upper bound (to within 7
   * decimal places).
   */
  float const durationKnobCurvature = 0.8018017;

  const std::array<Range const *, 3> ranges{&shortRange, &mediumRange, &longRange};

  auto range(float switchPosition) -> Range const * { return ranges[static_cast<int>(switchPosition)]; }

  auto rotationToTaper() -> std::function<float(float)> {
    return [](float rotation) -> float { return sigmoid::j_taper(rotation, durationKnobCurvature); };
  }

  auto taperToRotation() -> std::function<float(float)> {
    return [](float rotation) -> float { return sigmoid::j_taper(rotation, -durationKnobCurvature); };
  }

  auto withFixedRange(rack::engine::Module *module, int knobId, Range const &range) -> std::function<float()> {
    using namespace control;
    auto const durationRotation = knob::rotation(module, knobId);
    auto const durationKnobTaper = duration::rotationToTaper();
    auto const toDurationRange = scale::toRange(range);
    return knob::scaled(durationRotation, durationKnobTaper, toDurationRange);
  }

  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int switchId) -> std::function<float()> {
    using namespace control;
    auto const durationRotation = knob::rotation(module, knobId, cvId);
    auto const durationKnobTaper = duration::rotationToTaper();
    auto const selectedDurationRange = range::selection<3>(module, switchId, duration::ranges);
    auto const toDurationRange = scale::toRange(selectedDurationRange);
    return knob::scaled(durationRotation, durationKnobTaper, toDurationRange);
  }

  class RangeSwitchParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValueString() -> std::string override {
      static auto const descriptions = std::array<std::string, 3>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};
      auto const selection = static_cast<int>(getValue());
      return descriptions[selection];
    }
  };

  class KnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override {
      static auto const durationTaperFor = duration::rotationToTaper();
      auto const rotation = getValue();
      return range()->scale(durationTaperFor(rotation));
    }

    void setDisplayValue(float durationSeconds) override {
      static auto const rotationFor = duration::taperToRotation();
      auto const durationTaper = range()->normalize(durationSeconds);
      setValue(rotationFor(durationTaper));
    }

    std::function<Range const *()> range;
  };

  void configKnob(rack::engine::Module *module, int knobId, std::function<Range const *()> const &getRange,
                  std::string const &name, float initialPosition) {
    module->configParam<KnobParamQuantity>(knobId, 0.f, 1.f, initialPosition, name, " s");
    auto knobParamQuantity = dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
    knobParamQuantity->range = getRange;
  }

  void configKnob(rack::engine::Module *module, int knobId, Range const &range, std::string const &name,
                  float initialPosition) {
    auto const getRange = [range]() -> Range const * { return &range; };
    configKnob(module, knobId, getRange, name, initialPosition);
  }

  void configKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name,
                  float initialPosition) {
    auto *switchParam = &module->params[switchId];
    auto const getRange = [switchParam]() -> Range const * {
      auto const selection = static_cast<int>(switchParam->getValue());
      return duration::ranges[selection];
    };
    configKnob(module, knobId, getRange, name, initialPosition);
  }

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name, int initialPosition) {
    module->configParam<RangeSwitchParamQuantity>(switchId, 0.f, 2.f, initialPosition, name);
  }
} // namespace duration
} // namespace dhe
