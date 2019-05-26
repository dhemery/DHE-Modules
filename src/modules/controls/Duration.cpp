#include "modules/controls/Duration.h"

#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"

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
  static auto const knobTaperCurvature = 0.8018017F;
  static auto const knobTaper = taper::FixedJTaper{knobTaperCurvature};

  const std::array<Range const *, 3> ranges{&shortRange, &mediumRange, &longRange};

  auto range(float switchPosition) -> Range const * { return ranges[static_cast<int>(switchPosition)]; }

  auto withFixedRange(rack::engine::Module *module, int knobId, Range const &range) -> std::function<float()> {
    auto const rotation = knob::rotation(module, knobId);
    return knob::taperedAndScaled(rotation, knobTaper, range);
  }

  auto withSelectableRange(rack::engine::Module *module, int knobId, int cvId, int switchId) -> std::function<float()> {
    auto const rotation = knob::rotation(module, knobId, cvId);
    auto const rangeSelector = range::selector<3>(module, switchId, duration::ranges);
    return knob::taperedAndScaled(rotation, knobTaper, rangeSelector);
  }

  class KnobParamQuantity : public rack::engine::ParamQuantity {
  public:
    auto getDisplayValue() -> float override {
      auto const rotation = getValue();
      auto const tapered = knobTaper.apply(rotation);
      return range()->scale(tapered);
    }

    void setDisplayValue(float durationSeconds) override {
      auto const tapered = range()->normalize(durationSeconds);
      auto const rotation = knobTaper.invert(tapered);
      setValue(rotation);
    }

    void setRangeSupplier(std::function<Range const *()> const &supplier) { this->range = supplier; }

  private:
    std::function<Range const *()> range;
  };

  void configKnob(rack::engine::Module *module, int knobId, std::function<Range const *()> const &getRange,
                  std::string const &name, float initialPosition) {
    module->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, initialPosition, name, " s");
    auto knobParamQuantity = dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
    knobParamQuantity->setRangeSupplier(getRange);
  }

  void configKnob(rack::engine::Module *module, int knobId, Range const &range, std::string const &name,
                  float initialRotation) {
    auto const getRange = [range]() -> Range const * { return &range; };
    configKnob(module, knobId, getRange, name, initialRotation);
  }

  void configKnob(rack::engine::Module *module, int knobId, int switchId, std::string const &name,
                  float initialRotation) {
    auto *switchParam = &module->params[switchId];
    auto const getRange = [switchParam]() -> Range const * {
      auto const selection = static_cast<int>(switchParam->getValue());
      return duration::ranges[selection];
    };
    configKnob(module, knobId, getRange, name, initialRotation);
  }

  void configSwitch(rack::engine::Module *module, int switchId, std::string const &name, int initialPosition) {
    static auto const positionNames = std::array<std::string, 3>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};
    toggle::config<3>(module, switchId, name, positionNames, initialPosition);
  }
} // namespace duration
} // namespace dhe
