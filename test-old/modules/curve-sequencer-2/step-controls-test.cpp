#include "components/taper.h"
#include "controls/curvature-inputs.h"
#include "controls/duration-inputs.h"
#include "controls/level-inputs.h"
#include "fake-rack/Controls.h"
#include "modules/curve-sequencer-2/controls.h"
#include "modules/curve-sequencer-2/source.h"

#include <doctest.h>

namespace test {
namespace curve_sequencer_2 {
namespace step_controls {
static auto constexpr stepCount{8};
using Controls = dhe::curve_sequencer_2::Controls<fake::Port, fake::Param,
                                                  fake::Light, stepCount>;
using dhe::curve_sequencer_2::Source;
using dhe::curve_sequencer_2::TriggerMode;

TEST_CASE("curve_sequencer_2::StepControls") {
  std::vector<fake::Port> inputs{Controls::InputCount};
  std::vector<fake::Port> outputs{Controls::OutputCount};
  std::vector<fake::Param> params{Controls::ParameterCount};
  std::vector<fake::Light> lights{Controls::LightCount};

  Controls controls{inputs, outputs, params, lights};

  SUBCASE("triggerMode(step) is the mode selected by the step trigger mode "
          "switch") {
    auto constexpr step = 0;

    auto modeSelectedBySwitch = TriggerMode::GateIsHigh;
    params[Controls::TriggerModeSwitches + step].setValue(
        static_cast<float>(modeSelectedBySwitch));
    CHECK_EQ(controls.trigger_mode(step), modeSelectedBySwitch);

    modeSelectedBySwitch = TriggerMode::GateIsLow;
    params[Controls::TriggerModeSwitches + step].setValue(
        static_cast<float>(modeSelectedBySwitch));
    CHECK_EQ(controls.trigger_mode(step), modeSelectedBySwitch);

    modeSelectedBySwitch = TriggerMode::GateRises;
    params[Controls::TriggerModeSwitches + step].setValue(
        static_cast<float>(modeSelectedBySwitch));
    CHECK_EQ(controls.trigger_mode(step), modeSelectedBySwitch);

    modeSelectedBySwitch = TriggerMode::GateFalls;
    params[Controls::TriggerModeSwitches + step].setValue(
        static_cast<float>(modeSelectedBySwitch));
    CHECK_EQ(controls.trigger_mode(step), modeSelectedBySwitch);

    modeSelectedBySwitch = TriggerMode::GateChanges;
    params[Controls::TriggerModeSwitches + step].setValue(
        static_cast<float>(modeSelectedBySwitch));
    CHECK_EQ(controls.trigger_mode(step), modeSelectedBySwitch);
  }

  SUBCASE("interruptOnTrigger(step)") {
    auto constexpr step = 6;
    params[Controls::OnInterruptSwitches + step].setValue(0.F); // NOINT
    CHECK_FALSE(controls.interrupt_on_trigger(step));

    params[Controls::OnInterruptSwitches + step].setValue(1.F); // NEXT
    CHECK(controls.interrupt_on_trigger(step));
  }

  SUBCASE("advanceOnEndOfCurve(step)") {
    auto constexpr step = 6;
    params[Controls::OnEndOfCurveSwitches + step].setValue(0.F); // NOINT
    CHECK_FALSE(controls.advance_on_end_of_curve(step));

    params[Controls::OnEndOfCurveSwitches + step].setValue(1.F); // NEXT
    CHECK(controls.advance_on_end_of_curve(step));
  }

  SUBCASE("trackStartSource(step)") {
    auto constexpr step = 6;
    params[Controls::TrackStartSwitches + step].setValue(0.F); // SNAP
    CHECK_FALSE(controls.track_start_source(step));

    params[Controls::TrackStartSwitches + step].setValue(1.F); // TRACK
    CHECK(controls.track_start_source(step));
  }

  SUBCASE("startSource(step) is the source selected by the step start source "
          "switch") {
    auto constexpr step = 7;

    auto sourceSelectedBySwitch = Source::Level;
    params[Controls::StartSourceSwitches + step].setValue(
        static_cast<float>(sourceSelectedBySwitch));
    CHECK_EQ(controls.start_source(step), sourceSelectedBySwitch);

    sourceSelectedBySwitch = Source::In;
    params[Controls::StartSourceSwitches + step].setValue(
        static_cast<float>(sourceSelectedBySwitch));
    CHECK_EQ(controls.start_source(step), sourceSelectedBySwitch);

    sourceSelectedBySwitch = Source::Out;
    params[Controls::StartSourceSwitches + step].setValue(
        static_cast<float>(sourceSelectedBySwitch));
    CHECK_EQ(controls.start_source(step), sourceSelectedBySwitch);
  }

  SUBCASE("startLevel(step) reports level for sequence level switch and step "
          "start level param") {
    auto constexpr step = 3;
    auto constexpr startLevelKnobRotation = 0.35F;
    auto constexpr levelRangeSelection = 1; // unipolar

    params[Controls::StartLevelKnobs + step].setValue(startLevelKnobRotation);
    params[Controls::LevelRangeSwitch].setValue(
        static_cast<float>(levelRangeSelection));

    CHECK_EQ(controls.start_level(step),
             dhe::level(startLevelKnobRotation,
                        *dhe::signal_ranges[levelRangeSelection]));
  }

  SUBCASE("endSource(step) is the source selected by the step start source "
          "switch") {
    auto constexpr step = 7;

    auto sourceSelectedBySwitch = Source::Level;
    params[Controls::EndSourceSwitches + step].setValue(
        static_cast<float>(sourceSelectedBySwitch));
    CHECK_EQ(controls.end_source(step), sourceSelectedBySwitch);

    sourceSelectedBySwitch = Source::In;
    params[Controls::EndSourceSwitches + step].setValue(
        static_cast<float>(sourceSelectedBySwitch));
    CHECK_EQ(controls.end_source(step), sourceSelectedBySwitch);

    sourceSelectedBySwitch = Source::Out;
    params[Controls::EndSourceSwitches + step].setValue(
        static_cast<float>(sourceSelectedBySwitch));
    CHECK_EQ(controls.end_source(step), sourceSelectedBySwitch);
  }

  SUBCASE("endLevel(step) reports level for sequence level switch and step end "
          "level param") {
    auto constexpr step = 3;
    auto constexpr endLevelKnobRotation = 0.35F;
    auto constexpr levelRangeSelection = 1; // unipolar

    params[Controls::EndLevelKnobs + step].setValue(endLevelKnobRotation);
    params[Controls::LevelRangeSwitch].setValue(
        static_cast<float>(levelRangeSelection));

    CHECK_EQ(controls.end_level(step),
             dhe::level(endLevelKnobRotation,
                        *dhe::signal_ranges[levelRangeSelection]));
  }

  SUBCASE("trackEndSource(step)") {
    auto constexpr step = 1;
    params[Controls::TrackEndSwitches + step].setValue(0.F); // SNAP
    CHECK_FALSE(controls.track_end_source(step));

    params[Controls::TrackEndSwitches + step].setValue(1.F); // TRACK
    CHECK(controls.track_end_source(step));
  }

  SUBCASE("taper(step) is the taper selected by step shape switch") {
    auto constexpr step = 3;

    auto shapeSelection = 0; // J
    params[Controls::ShapeSwitches + step].setValue(
        static_cast<float>(shapeSelection));
    CHECK_EQ(controls.taper(step), dhe::taper::tapers[shapeSelection]);

    shapeSelection = 1; // S
    params[Controls::ShapeSwitches + step].setValue(
        static_cast<float>(shapeSelection));
    CHECK_EQ(controls.taper(step), dhe::taper::tapers[shapeSelection]);
  }

  SUBCASE("curvature(step) reports curvature for the step curvature param") {
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
                           *dhe::duration_ranges[durationRangeSelection]));
  }

  SUBCASE("showInactive(step) dims step progress lights") {
    auto constexpr step = 3;
    auto constexpr completedProgressLightIndex = step + step;
    auto constexpr remainingProgressLightIndex = step + step + 1;

    controls.show_inactive(step);

    CHECK_EQ(lights[Controls::ProgressLights + completedProgressLightIndex]
                 .getBrightness(),
             0.F);
    CHECK_EQ(lights[Controls::ProgressLights + remainingProgressLightIndex]
                 .getBrightness(),
             0.F);
  }

  SUBCASE("isEnabled(step)") {
    SUBCASE("true if enabled button is pressed") {
      auto constexpr step = 3;
      params[Controls::EnabledButtons + step].setValue(1.F);

      CHECK(controls.is_enabled(step));
    }

    SUBCASE("true if enabled input is high") {
      auto constexpr step = 6;
      inputs[Controls::EnabledInputs + step].setVoltage(10.F);

      CHECK(controls.is_enabled(step));
    }

    SUBCASE("false if enabled button is not pressed and enabled input is low") {
      auto constexpr step = 7;

      params[Controls::EnabledButtons + step].setValue(0.F);
      inputs[Controls::EnabledInputs + step].setVoltage(0.F);

      CHECK_FALSE(controls.is_enabled(step));
    }
  }
}
} // namespace step_controls
} // namespace curve_sequencer_2
} // namespace test
