#include "modules/curve-sequencer/CurveSequencerControls.h"

#include "components/Taper.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"
#include "doctest/doctest.h"
#include "modules/curve-sequencer/GenerateMode.h"

namespace test {
namespace curve_sequencer_controls {

TEST_CASE("curve_sequencer::CurveSequencerControls") {
  static auto constexpr stepCount{8};

  using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;
  using dhe::curve_sequencer::AdvanceMode;
  using dhe::curve_sequencer::GenerateMode;

  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Output> outputs{Controls::OutputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  std::vector<rack::engine::Light> lights{Controls::LightCount};

  Controls controls{inputs, outputs, params, lights};

  SUBCASE("curvature(step)  reports curvature for step curvature param") {
    auto constexpr step = 5;
    auto constexpr curveKnobRotation = 0.3F;
    params[Controls::CurveKnobs + step].setValue(curveKnobRotation);

    CHECK_EQ(controls.curvature(step), dhe::curvature(curveKnobRotation));
  }

  SUBCASE("duration(step) reports duration for duration range switch and step "
          "duration param") {
    auto constexpr step = 7;
    auto constexpr durationKnobRotation = 0.75F;
    auto constexpr durationRangeSelection = 2; // Long duration

    params[Controls::DurationKnobs + step].setValue(durationKnobRotation);
    params[Controls::DurationRangeSwitch].setValue(
        static_cast<float>(durationRangeSelection));

    CHECK_EQ(controls.duration(step),
             dhe::duration(durationKnobRotation,
                           *dhe::durationRanges[durationRangeSelection]));
  }

  SUBCASE(
      "level(step) reports level for level range switch and step level param") {
    auto constexpr step = 3;
    auto constexpr levelKnobRotation = 0.35F;
    auto constexpr levelRangeSelection = 1; // unipolar

    params[Controls::LevelKnobs + step].setValue(levelKnobRotation);
    params[Controls::LevelRangeSwitch].setValue(
        static_cast<float>(levelRangeSelection));

    CHECK_EQ(
        controls.level(step),
        dhe::level(levelKnobRotation, *dhe::signalRanges[levelRangeSelection]));
  }

  SUBCASE("taper(step) is taper selected by step shape switch") {
    auto constexpr step = 3;

    auto shapeSelection = 0; // J
    params[Controls::ShapeSwitches + step].setValue(
        static_cast<float>(shapeSelection));
    CHECK_EQ(controls.taper(step), dhe::taper::variableTapers[shapeSelection]);

    shapeSelection = 1; // S
    params[Controls::ShapeSwitches + step].setValue(
        static_cast<float>(shapeSelection));
    CHECK_EQ(controls.taper(step), dhe::taper::variableTapers[shapeSelection]);
  }

  SUBCASE("mode(step) is mode selected by step mode switch") {
    auto constexpr step = 6;

    auto modeSelectedBySwitch = GenerateMode::Curve;
    params[Controls::ModeSwitches + step].setValue(
        static_cast<float>(modeSelectedBySwitch));
    CHECK_EQ(controls.mode(step), modeSelectedBySwitch);

    modeSelectedBySwitch = GenerateMode::Hold;
    params[Controls::ModeSwitches + step].setValue(
        static_cast<float>(modeSelectedBySwitch));
    CHECK_EQ(controls.mode(step), modeSelectedBySwitch);
  }

  SUBCASE("condition(step) is condition selected by step condition switch") {
    auto constexpr step = 0;

    auto conditionSelectedBySwitch = AdvanceMode::TimerExpires;
    params[Controls::ConditionSwitches + step].setValue(
        static_cast<float>(conditionSelectedBySwitch));
    CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateIsHigh;
    params[Controls::ConditionSwitches + step].setValue(
        static_cast<float>(conditionSelectedBySwitch));
    CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateIsLow;
    params[Controls::ConditionSwitches + step].setValue(
        static_cast<float>(conditionSelectedBySwitch));
    CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateRises;
    params[Controls::ConditionSwitches + step].setValue(
        static_cast<float>(conditionSelectedBySwitch));
    CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateFalls;
    params[Controls::ConditionSwitches + step].setValue(
        static_cast<float>(conditionSelectedBySwitch));
    CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);

    conditionSelectedBySwitch = AdvanceMode::GateChanges;
    params[Controls::ConditionSwitches + step].setValue(
        static_cast<float>(conditionSelectedBySwitch));
    CHECK_EQ(controls.condition(step), conditionSelectedBySwitch);
  }

  SUBCASE("showInactive(step) dims step progress lights") {
    auto constexpr step = 3;
    auto constexpr completedProgressLightIndex = step + step;
    auto constexpr remainingProgressLightIndex = step + step + 1;

    controls.showInactive(step);

    CHECK_EQ(lights[Controls::ProgressLights + completedProgressLightIndex]
                 .getBrightness(),
             0.F);
    CHECK_EQ(lights[Controls::ProgressLights + remainingProgressLightIndex]
                 .getBrightness(),
             0.F);
  }
}
} // namespace curve_sequencer_controls
} // namespace test
