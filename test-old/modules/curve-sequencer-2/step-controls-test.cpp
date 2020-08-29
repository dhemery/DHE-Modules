#include "components/sigmoid.h"
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
static auto constexpr step_count{8};
using Controls = dhe::curve_sequencer_2::Controls<fake::Port, fake::Param,
                                                  fake::Light, step_count>;
using dhe::enum_index;
using dhe::curve_sequencer_2::Source;

TEST_CASE("curve_sequencer_2::StepControls") {
  std::vector<fake::Port> inputs{enum_index(Controls::Input::Count)};
  std::vector<fake::Port> outputs{enum_index(Controls::Output::Count)};
  std::vector<fake::Param> params{enum_index(Controls::Param::Count)};
  std::vector<fake::Light> lights{enum_index(Controls::Light::Count)};

  Controls controls{inputs, outputs, params, lights};

  SUBCASE("trackStartSource(step)") {
    auto constexpr step = 6;
    params[enum_index(Controls::Param::StepTracksStartSource, step)].setValue(
        0.F); // SNAP
    CHECK_FALSE(controls.track_start_source(step));

    params[enum_index(Controls::Param::StepTracksStartSource, step)].setValue(
        1.F); // TRACK
    CHECK(controls.track_start_source(step));
  }

  SUBCASE("startSource(step) is the source selected by the step start source "
          "switch") {
    auto constexpr step = 7;

    auto source_selected_by_switch = Source::Level;
    params[enum_index(Controls::Param::StepStartSource, step)].setValue(
        static_cast<float>(source_selected_by_switch));
    CHECK_EQ(controls.start_source(step), source_selected_by_switch);

    source_selected_by_switch = Source::In;
    params[enum_index(Controls::Param::StepStartSource, step)].setValue(
        static_cast<float>(source_selected_by_switch));
    CHECK_EQ(controls.start_source(step), source_selected_by_switch);

    source_selected_by_switch = Source::Out;
    params[enum_index(Controls::Param::StepStartSource, step)].setValue(
        static_cast<float>(source_selected_by_switch));
    CHECK_EQ(controls.start_source(step), source_selected_by_switch);
  }

  SUBCASE("startLevel(step) reports level for sequence level switch and step "
          "start level param") {
    auto constexpr step = 3;
    auto constexpr start_level_knob_rotation = 0.35F;
    auto constexpr level_range_selection = 1; // unipolar

    params[enum_index(Controls::Param::StepStartLevel, step)].setValue(
        start_level_knob_rotation);
    params[enum_index(Controls::Param::LevelRange)].setValue(
        static_cast<float>(level_range_selection));

    CHECK_EQ(controls.start_level(step),
             dhe::level(start_level_knob_rotation,
                        dhe::signal_ranges[level_range_selection]));
  }

  SUBCASE("endSource(step) is the source selected by the step start source "
          "switch") {
    auto constexpr step = 7;

    auto source_selected_by_switch = Source::Level;
    params[enum_index(Controls::Param::StepEndSource, step)].setValue(
        static_cast<float>(source_selected_by_switch));
    CHECK_EQ(controls.end_source(step), source_selected_by_switch);

    source_selected_by_switch = Source::In;
    params[enum_index(Controls::Param::StepEndSource, step)].setValue(
        static_cast<float>(source_selected_by_switch));
    CHECK_EQ(controls.end_source(step), source_selected_by_switch);

    source_selected_by_switch = Source::Out;
    params[enum_index(Controls::Param::StepEndSource, step)].setValue(
        static_cast<float>(source_selected_by_switch));
    CHECK_EQ(controls.end_source(step), source_selected_by_switch);
  }

  SUBCASE("endLevel(step) reports level for sequence level switch and step end "
          "level param") {
    auto constexpr step = 3;
    auto constexpr end_level_knob_rotation = 0.35F;
    auto constexpr level_range_selection = 1; // unipolar

    params[enum_index(Controls::Param::StepEndLevel, step)].setValue(
        end_level_knob_rotation);
    params[enum_index(Controls::Param::LevelRange)].setValue(
        static_cast<float>(level_range_selection));

    CHECK_EQ(controls.end_level(step),
             dhe::level(end_level_knob_rotation,
                        dhe::signal_ranges[level_range_selection]));
  }

  SUBCASE("trackEndSource(step)") {
    auto constexpr step = 1;
    params[enum_index(Controls::Param::StepTracksEndSource, step)].setValue(
        0.F); // SNAP
    CHECK_FALSE(controls.track_end_source(step));

    params[enum_index(Controls::Param::StepTracksEndSource, step)].setValue(
        1.F); // TRACK
    CHECK(controls.track_end_source(step));
  }

  SUBCASE("taper(step) is the taper selected by step shape switch") {
    auto constexpr step = 3;

    auto shape_selection = 0; // J
    params[enum_index(Controls::Param::StepShape, step)].setValue(
        static_cast<float>(shape_selection));
    CHECK_EQ(controls.taper(step), dhe::sigmoid::tapers[shape_selection]);

    shape_selection = 1; // S
    params[enum_index(Controls::Param::StepShape, step)].setValue(
        static_cast<float>(shape_selection));
    CHECK_EQ(controls.taper(step), dhe::sigmoid::tapers[shape_selection]);
  }

  SUBCASE("curvature(step) reports curvature for the step curvature param") {
    auto constexpr step = 5;
    auto constexpr curve_knob_rotation = 0.3F;
    params[enum_index(Controls::Param::StepCurvature, step)].setValue(
        curve_knob_rotation);

    CHECK_EQ(controls.curvature(step), dhe::curvature(curve_knob_rotation));
  }

  SUBCASE("duration(step) reports duration for duration range switch and step "
          "duration param") {
    auto constexpr step = 7;
    auto constexpr duration_knob_rotation = 0.75F;
    auto constexpr duration_range_selection = 2; // Long duration

    params[enum_index(Controls::Param::StepDuration, step)].setValue(
        duration_knob_rotation);
    params[enum_index(Controls::Param::DurationRange)].setValue(
        static_cast<float>(duration_range_selection));

    CHECK_EQ(controls.duration(step),
             dhe::duration(duration_knob_rotation,
                           dhe::duration_ranges[duration_range_selection]));
  }

  SUBCASE("showInactive(step) dims step progress lights") {
    auto constexpr step = 3;
    auto constexpr completed_progress_light_index = step + step;
    auto constexpr remaining_progress_light_index = step + step + 1;

    controls.show_inactive(step);

    CHECK_EQ(lights[enum_index(Controls::Light::StepProgress,
                               completed_progress_light_index)]
                 .getBrightness(),
             0.F);
    CHECK_EQ(lights[enum_index(Controls::Light::StepProgress,
                               remaining_progress_light_index)]
                 .getBrightness(),
             0.F);
  }

  SUBCASE("isEnabled(step)") {
    SUBCASE("true if enabled button is pressed") {
      auto constexpr step = 3;
      params[enum_index(Controls::Param::EnableStep, step)].setValue(1.F);

      CHECK(controls.is_enabled(step));
    }

    SUBCASE("true if enabled input is high") {
      auto constexpr step = 6;
      inputs[enum_index(Controls::Input::EnableStep, step)].setVoltage(10.F);

      CHECK(controls.is_enabled(step));
    }

    SUBCASE("false if enabled button is not pressed and enabled input is low") {
      auto constexpr step = 7;

      params[enum_index(Controls::Param::EnableStep, step)].setValue(0.F);
      inputs[enum_index(Controls::Input::EnableStep, step)].setVoltage(0.F);

      CHECK_FALSE(controls.is_enabled(step));
    }
  }
}
} // namespace step_controls
} // namespace curve_sequencer_2
} // namespace test
