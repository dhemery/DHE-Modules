#include "components/Taper.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"
#include "doctest/doctest.h"
#include "fake/rack-controls.h"
#include "modules/curve-sequencer-2/CurveSequencer2Controls.h"

namespace test {
namespace curve_sequencer_controls {

  /**
   * TRIG: RISE FALL EDGE HIGH LOW
   * INT: NOINT NEXT
   * EOS: HOLD NEXT
   * START SOURCE: OUT IN LEVEL
   * START LEVEL
   * START SOURCE TRACKING: SNAP TRACK
   * END SOURCE: OUT IN LEVEL
   * END LEVEL
   * END SOURCE TRACKING: SNAP TRACK
   * DURATION
   * SHAPE
   * CURVATURE
   * ON
   */
  TEST_CASE("curve_sequencer2::CurveSequencer2StepControls") {
    static auto constexpr stepCount{8};

    using Controls = dhe::curve_sequencer::CurveSequencer2Controls<fake::Port, fake::Param, fake::Light, stepCount>;
    using dhe::curve_sequencer::TriggerMode;

    std::vector<fake::Port> inputs{Controls::InputCount};
    std::vector<fake::Port> outputs{Controls::OutputCount};
    std::vector<fake::Param> params{Controls::ParameterCount};
    std::vector<fake::Light> lights{Controls::LightCount};

    Controls controls{inputs, outputs, params, lights};

    SUBCASE("isEnabled(step)") {
      SUBCASE("true if enabled button is pressed") {
        auto constexpr step = 3;
        params[Controls::EnabledButtons + step].setValue(1.F);

        CHECK(controls.isEnabled(step));
      }

      SUBCASE("true if enabled input is high") {
        auto constexpr step = 6;
        inputs[Controls::EnabledInputs + step].setVoltage(10.F);

        CHECK(controls.isEnabled(step));
      }

      SUBCASE("false if enabled button is not pressed and enabled input is low") {
        auto constexpr step = 7;

        params[Controls::EnabledButtons + step].setValue(0.F);
        inputs[Controls::EnabledInputs + step].setVoltage(0.F);

        CHECK_FALSE(controls.isEnabled(step));
      }
    }

    SUBCASE("curvature(step) reports curvature for the step curvature param") {
      auto constexpr step = 5;
      auto constexpr curveKnobRotation = 0.3F;
      params[Controls::CurveKnobs + step].setValue(curveKnobRotation);

      CHECK_EQ(controls.curvature(step), dhe::curvature(curveKnobRotation));
    }

    SUBCASE("duration(step) reports duration for duration range switch and step duration param") {
      auto constexpr step = 7;
      auto constexpr durationKnobRotation = 0.75F;
      auto constexpr durationRangeSelection = 2; // Long duration

      params[Controls::DurationKnobs + step].setValue(durationKnobRotation);
      params[Controls::DurationRangeSwitch].setValue(static_cast<float>(durationRangeSelection));

      CHECK_EQ(controls.duration(step),
               dhe::duration(durationKnobRotation, *dhe::durationRanges[durationRangeSelection]));
    }

    SUBCASE("level(step) reports level for level switch and step level param") {
      auto constexpr step = 3;
      auto constexpr levelKnobRotation = 0.35F;
      auto constexpr levelRangeSelection = 1; // unipolar

      params[Controls::LevelKnobs + step].setValue(levelKnobRotation);
      params[Controls::LevelRangeSwitch].setValue(static_cast<float>(levelRangeSelection));

      CHECK_EQ(controls.level(step), dhe::level(levelKnobRotation, *dhe::signalRanges[levelRangeSelection]));
    }

    SUBCASE("taper(step) is the taper selected by step shape switch") {
      auto constexpr step = 3;

      auto shapeSelection = 0; // J
      params[Controls::ShapeSwitches + step].setValue(static_cast<float>(shapeSelection));
      CHECK_EQ(controls.taper(step), dhe::taper::variableTapers[shapeSelection]);

      shapeSelection = 1; // S
      params[Controls::ShapeSwitches + step].setValue(static_cast<float>(shapeSelection));
      CHECK_EQ(controls.taper(step), dhe::taper::variableTapers[shapeSelection]);
    }

    SUBCASE("triggerMode(step) is the mode selected by the step trigger mode picker") {
      auto constexpr step = 0;

      auto conditionSelectedBySwitch = TriggerMode::GateIsHigh;
      params[Controls::TriggerModePickers + step].setValue(static_cast<float>(conditionSelectedBySwitch));
      CHECK_EQ(controls.triggerMode(step), conditionSelectedBySwitch);

      conditionSelectedBySwitch = TriggerMode::GateIsLow;
      params[Controls::TriggerModePickers + step].setValue(static_cast<float>(conditionSelectedBySwitch));
      CHECK_EQ(controls.triggerMode(step), conditionSelectedBySwitch);

      conditionSelectedBySwitch = TriggerMode::GateRises;
      params[Controls::TriggerModePickers + step].setValue(static_cast<float>(conditionSelectedBySwitch));
      CHECK_EQ(controls.triggerMode(step), conditionSelectedBySwitch);

      conditionSelectedBySwitch = TriggerMode::GateFalls;
      params[Controls::TriggerModePickers + step].setValue(static_cast<float>(conditionSelectedBySwitch));
      CHECK_EQ(controls.triggerMode(step), conditionSelectedBySwitch);

      conditionSelectedBySwitch = TriggerMode::GateChanges;
      params[Controls::TriggerModePickers + step].setValue(static_cast<float>(conditionSelectedBySwitch));
      CHECK_EQ(controls.triggerMode(step), conditionSelectedBySwitch);
    }

    SUBCASE("showInactive(step) dims step progress lights") {
      auto constexpr step = 3;
      auto constexpr completedProgressLightIndex = step + step;
      auto constexpr remainingProgressLightIndex = step + step + 1;

      controls.showInactive(step);

      CHECK_EQ(lights[Controls::ProgressLights + completedProgressLightIndex].getBrightness(), 0.F);
      CHECK_EQ(lights[Controls::ProgressLights + remainingProgressLightIndex].getBrightness(), 0.F);
    }
  }
} // namespace curve_sequencer_controls
} // namespace test
