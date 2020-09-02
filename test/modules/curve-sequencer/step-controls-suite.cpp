#include "./fixtures/controls-fixture.h"
#include "components/sigmoid.h"
#include "modules/curve-sequencer/controls.h"
#include "modules/curve-sequencer/triggers.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>
#include <functional>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::Source;
using dhe::curve_sequencer::TriggerMode;
using dhe::unit::is_between;
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_near;
using dhe::unit::is_no_greater_than;
using dhe::unit::is_no_less_than;
using dhe::unit::is_true;
using dhe::unit::Suite;

auto trigger_mode_test(TriggerMode mode) -> TestFunc {
  return test([mode](Tester &t, Module &module, Controls &controls) {
    auto constexpr step = 4;
    auto const switch_value = static_cast<float>(mode);

    module.set_param(Controls::Param::StepTriggerMode, step, switch_value);

    t.assert_that(controls.trigger_mode(step), is_equal_to(mode));
  });
}

class StepControlsSuite : Suite {
public:
  StepControlsSuite() : Suite{"dhe::curve_sequencer::Controls/step"} {}
  void register_tests(dhe::unit::TestRegistrar add_test) override {
    for (auto trigger_mode : dhe::curve_sequencer::trigger_modes) {
      auto const name =
          std::string{"trigger_mode(s): "} + name_of(trigger_mode);
      add_test(name, trigger_mode_test(trigger_mode));
    }

    add_test(
        "interrupt_on_trigger(s)",
        test([](Tester &t, Module &module, Controls controls) {
          auto constexpr step = 3;
          module.set_param(Controls::Param::InterruptStepOnTrigger, step, 0.F);

          t.assert_that("interrupt disabled",
                        controls.interrupt_on_trigger(step), is_false);

          module.set_param(Controls::Param::InterruptStepOnTrigger, step, 1.F);

          t.assert_that("interrupt enabled",
                        controls.interrupt_on_trigger(step), is_true);
        }));

    add_test(
        "advance_on_end_of_curve(s)",
        test([](Tester &t, Module &module, Controls controls) {
          auto constexpr step = 3;
          module.set_param(Controls::Param::AdvanceStepOnEndOfCurve, step, 0.F);

          t.assert_that("advance disabled",
                        controls.advance_on_end_of_curve(step), is_false);

          module.set_param(Controls::Param::AdvanceStepOnEndOfCurve, step, 1.F);

          t.assert_that("advance enabled",
                        controls.advance_on_end_of_curve(step), is_true);
        }));

    add_test("start_source(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 0;

               auto source = Source::Level;
               module.set_param(Controls::Param::StepStartSource, step,
                                static_cast<float>(source));

               t.assert_that("Source::Level", controls.start_source(step),
                             is_equal_to(source));

               source = Source::In;
               module.set_param(Controls::Param::StepStartSource, step,
                                static_cast<float>(source));

               t.assert_that("Source::In", controls.start_source(step),
                             is_equal_to(source));

               source = Source::Out;
               module.set_param(Controls::Param::StepStartSource, step,
                                static_cast<float>(source));

               t.assert_that("Source::Out", controls.start_source(step),
                             is_equal_to(source));
             }));

    add_test(
        "track_start_source(s)",
        test([](Tester &t, Module &module, Controls controls) {
          auto constexpr step = 5;

          module.set_param(Controls::Param::StepTracksStartSource, step, 1.F);

          t.assert_that("tracking enabled", controls.track_start_source(step),
                        is_true);

          module.set_param(Controls::Param::StepTracksStartSource, step, 0.F);

          t.assert_that("tracking disabled", controls.track_start_source(step),
                        is_false);
        }));

    add_test(
        "start_level(s)",
        test([](Tester &t, Module &module, Controls controls) {
          auto constexpr step = 7;
          auto constexpr rotation = 0.37F;
          auto constexpr range = 1; // unipolar 0–10
          module.set_param(Controls::Param::StepStartLevel, step, rotation);
          module.set_param(Controls::Param::LevelRange, range);

          t.assert_that(
              controls.start_level(step),
              is_equal_to(dhe::level(rotation, dhe::signal_ranges[range])));
        }));

    add_test("end_source(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 6;

               auto source = Source::Level;
               module.set_param(Controls::Param::StepEndSource, step,
                                static_cast<float>(source));

               t.assert_that("Source::Level", controls.end_source(step),
                             is_equal_to(source));

               source = Source::In;
               module.set_param(Controls::Param::StepEndSource, step,
                                static_cast<float>(source));

               t.assert_that("Source::In", controls.end_source(step),
                             is_equal_to(source));

               source = Source::Out;
               module.set_param(Controls::Param::StepEndSource, step,
                                static_cast<float>(source));

               t.assert_that("Source::Out", controls.end_source(step),
                             is_equal_to(source));
             }));

    add_test(
        "track_end_source(s)",
        test([](Tester &t, Module &module, Controls controls) {
          auto constexpr step = 5;

          module.set_param(Controls::Param::StepTracksEndSource, step, 1.F);

          t.assert_that("tracking enabled", controls.track_end_source(step),
                        is_true);

          module.set_param(Controls::Param::StepTracksEndSource, step, 0.F);

          t.assert_that("tracking disabled", controls.track_end_source(step),
                        is_false);
        }));

    add_test("end_level(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 2;
               auto constexpr rotation = 0.22F;
               auto constexpr range = 0;
               module.set_param(Controls::Param::StepEndLevel, step, rotation);
               module.set_param(Controls::Param::LevelRange, range);

               auto want = dhe::level(rotation, dhe::signal_ranges[range]);

               t.assert_that(controls.end_level(step), is_equal_to(want));
             }));

    add_test("duration(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 2;
               auto constexpr rotation = 0.22F;
               auto constexpr range = 0;
               module.set_param(Controls::Param::StepDuration, step, rotation);
               module.set_param(Controls::Param::DurationRange, range);

               auto want = dhe::duration(rotation, dhe::duration_ranges[range]);

               t.assert_that(controls.duration(step), is_equal_to(want));
             }));

    add_test("taper(s)", test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 3;

               auto selection = 0; // J
               module.set_param(Controls::Param::StepShape, step,
                                static_cast<float>(selection));

               t.assert_that("J", controls.taper(step),
                             is_equal_to(dhe::sigmoid::tapers[selection]));

               selection = 1; // S
               module.set_param(Controls::Param::StepShape, step,
                                static_cast<float>(selection));
               t.assert_that("S", controls.taper(step),
                             is_equal_to(dhe::sigmoid::tapers[selection]));
             }));

    add_test("curvature(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 5;
               auto constexpr rotation = 0.68F;

               module.set_param(Controls::Param::StepCurvature, step, rotation);

               t.assert_that(controls.curvature(step),
                             is_near(dhe::curvature(rotation), 0.00001F));
             }));

    add_test(
        "show_progress(s)",
        test([](Tester &t, Module &module, Controls controls) {
          auto constexpr step = 6;

          auto constexpr completed_index = step + step;
          auto constexpr remaining_index = step + step + 1;

          controls.show_progress(step, 0.4F);

          auto const early_completed =
              module.get_light(Controls::Light::StepProgress, completed_index);

          auto const early_remaining =
              module.get_light(Controls::Light::StepProgress, remaining_index);

          t.assert_that("early completed", early_completed,
                        is_no_less_than(0.F));

          t.assert_that("early remaining", early_remaining,
                        is_no_greater_than(1.F));

          controls.show_progress(step, 0.6F);

          auto const late_completed =
              module.get_light(Controls::Light::StepProgress, completed_index);
          auto const late_remaining =
              module.get_light(Controls::Light::StepProgress, remaining_index);

          t.assert_that("late completed", late_completed,
                        is_between(early_completed, 1.F));

          t.assert_that("late remaining", late_remaining,
                        is_between(0.F, early_remaining));
        }));

    add_test("show_inactive(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 5;

               auto constexpr completed_index = step + step;
               auto constexpr remaining_index = step + step + 1;

               controls.show_inactive(step);

               auto const completed = module.get_light(
                   Controls::Light::StepProgress, completed_index);
               auto const remaining = module.get_light(
                   Controls::Light::StepProgress, remaining_index);

               t.assert_that("completed", completed, is_equal_to(0.F));
               t.assert_that("remaining", remaining, is_equal_to(0.F));
             }));

    add_test(
        "is_enabled(s)", test([](Tester &t, Module &module, Controls controls) {
          auto constexpr step = 3;

          module.set_param(Controls::Param::EnableStep, step, 1.F);
          module.set_input(Controls::Input::EnableStep, step, 0.F);

          t.assert_that("button pressed", controls.is_enabled(step), is_true);

          module.set_param(Controls::Param::EnableStep, step, 0.F);
          module.set_input(Controls::Input::EnableStep, step, 10.F);

          t.assert_that("input high", controls.is_enabled(step), is_true);

          module.set_param(Controls::Param::EnableStep, step, 0.F);
          module.set_input(Controls::Input::EnableStep, step, 0.F);

          t.assert_that("input low, button not pressed",
                        controls.is_enabled(step), is_false);
        }));
  }
};

__attribute__((unused)) static auto _ = StepControlsSuite{};
} // namespace curve_sequencer
} // namespace test
