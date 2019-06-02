#include "modules/controls/DurationConfig.h"

#include "modules/components/Taper.h"
#include "modules/controls/Functions.h"

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

  auto const shortRange = Range{0.001F, 1.F};
  auto const mediumRange = Range{0.01F, 10.F};
  auto const longRange = Range{0.1F, 100.F};

  const std::array<Range const *, 3> ranges{&shortRange, &mediumRange, &longRange};

  auto withMediumRange(rack::engine::Module const *module, int knobId) -> std::function<float()> {
    return taperedAndScaledRotationFunction(module, knobId, knobTaper, mediumRange);
  }

  auto withSelectableRange(rack::engine::Module const *module, int knobId, int cvId, int switchId)
      -> std::function<float()> {
    return taperedAndScaledRotationFunction<3>(module, knobId, cvId, knobTaper, switchId, duration::ranges);
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

  void configKnob(rack::engine::Module const *module, int knobId, std::function<Range const *()> const &getRange,
                  std::string const &name, float initialPosition) {
    nonConst(module)->configParam<KnobParamQuantity>(knobId, 0.F, 1.F, initialPosition, name, " s");
    auto knobParamQuantity = dynamic_cast<KnobParamQuantity *>(module->paramQuantities[knobId]);
    knobParamQuantity->setRangeSupplier(getRange);
  }

  void configKnob(rack::engine::Module const *module, int knobId, Range const &range, std::string const &name,
                  float initialRotation) {
    auto const getRange = [range]() -> Range const * { return &range; };
    configKnob(module, knobId, getRange, name, initialRotation);
  }

  void configKnob(rack::engine::Module const *module, int knobId, int switchId, std::string const &name,
                  float initialRotation) {
    auto const getRange = [module, switchId]() -> Range const * {
      auto const selection = switchPosition(module, switchId);
      return duration::ranges[selection];
    };
    configKnob(module, knobId, getRange, name, initialRotation);
  }

  void configSwitch(rack::engine::Module const *module, int switchId, std::string const &name, int initialPosition) {
    static auto const positionNames = std::array<std::string, 3>{"0.001–1.0 s", "0.01–10.0 s", "0.1–100.0 s"};
    configToggle<3>(module, switchId, name, positionNames, initialPosition);
  }
} // namespace duration
} // namespace dhe
