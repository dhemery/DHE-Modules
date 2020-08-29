#include "components/sigmoid.h"
#include "controls-test.h"
#include "modules/curve-sequencer-2/controls.h"
#include "modules/curve-sequencer-2/triggers.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer_2 {
namespace step_controls {
using dhe::curve_sequencer_2::Source;
using dhe::curve_sequencer_2::TriggerMode;
using dhe::unit::Suite;

auto trigger_mode_test(TriggerMode mode) -> TestFunc {
  return test([mode](Tester &t, Module &module, Controls &controls) {
    auto constexpr step = 4;
    auto const switch_value = static_cast<float>(mode);
    module.set(Controls::Param::StepTriggerMode, step, switch_value);
    TriggerMode got = controls.trigger_mode(step);
    if (got != mode) {
      t.errorf("got mode {}, want {}", got, mode);
    }
  });
}

class StepControlsTests : Suite {
public:
  StepControlsTests() : Suite{"dhe::curve_sequencer_2::[Step]Controls"} {}
  void register_tests(dhe::unit::TestRegistrar add_test) override {
    for (auto trigger_mode : dhe::curve_sequencer_2::trigger_modes) {
      auto const name =
          std::string{"trigger_mode(s): "} + name_of(trigger_mode);
      add_test(name, trigger_mode_test(trigger_mode));
    }

    add_test("interrupt_on_trigger(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 3;
               module.set(Controls::Param::InterruptStepOnTrigger, step, 0.F);
               auto interrupt_on_trigger = controls.interrupt_on_trigger(step);
               if (interrupt_on_trigger) {
                 t.errorf("with option disabled, got true, want false");
               }
               module.set(Controls::Param::InterruptStepOnTrigger, step, 1.F);
               interrupt_on_trigger = controls.interrupt_on_trigger(step);
               if (!interrupt_on_trigger) {
                 t.errorf("with option enabled, got false, want true");
               }
             }));

    add_test("advance_on_end_of_curve(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 3;
               module.set(Controls::Param::AdvanceStepOnEndOfCurve, step, 0.F);
               auto advance = controls.advance_on_end_of_curve(step);
               if (advance) {
                 t.errorf("with option disabled, got true, want false");
               }
               module.set(Controls::Param::AdvanceStepOnEndOfCurve, step, 1.F);
               advance = controls.advance_on_end_of_curve(step);
               if (!advance) {
                 t.errorf("with option enabled, got false, want true");
               }
             }));

    add_test("start_source(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 0;
               auto source = Source::Level;
               module.set(Controls::Param::StepStartSource, step,
                          static_cast<float>(source));
               auto got = controls.start_source(step);
               if (got != source) {
                 t.errorf("Source::Level: got {}, want {}", got, source);
               }

               source = Source::In;
               module.set(Controls::Param::StepStartSource, step,
                          static_cast<float>(source));
               got = controls.start_source(step);
               if (got != source) {
                 t.errorf("Source::In: got {}, want {}", got, source);
               }

               source = Source::Out;
               module.set(Controls::Param::StepStartSource, step,
                          static_cast<float>(source));
               got = controls.start_source(step);
               if (got != source) {
                 t.errorf("Source::Out: got {}, want {}", got, source);
               }
             }));

    add_test("track_start_source(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 5;
               module.set(Controls::Param::StepTracksStartSource, step, 1.F);
               auto track = controls.track_start_source(step);
               if (!track) {
                 t.error("with tracking enabled: got false, want true");
               }
               module.set(Controls::Param::StepTracksStartSource, step, 0.F);
               track = controls.track_start_source(step);
               if (track) {
                 t.error("with tracking disabled: got true, want false");
               }
             }));

    add_test("start_level(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 7;
               auto constexpr rotation = 0.37F;
               auto constexpr range = 1; // unipolar 0–10
               module.set(Controls::Param::StepStartLevel, step, rotation);
               module.set(Controls::Param::LevelRange, range);
               auto want = dhe::level(rotation, dhe::signal_ranges[range]);
               auto got = controls.start_level(step);
               if (got != want) {
                 t.errorf("got {}, want {}", got, want);
               }
             }));

    add_test("end_source(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 6;
               auto source = Source::Level;
               module.set(Controls::Param::StepEndSource, step,
                          static_cast<float>(source));
               auto got = controls.end_source(step);
               if (got != source) {
                 t.errorf("Source::Level: got {}, want {}", got, source);
               }

               source = Source::In;
               module.set(Controls::Param::StepEndSource, step,
                          static_cast<float>(source));
               got = controls.end_source(step);
               if (got != source) {
                 t.errorf("Source::In: got {}, want {}", got, source);
               }

               source = Source::Out;
               module.set(Controls::Param::StepEndSource, step,
                          static_cast<float>(source));
               got = controls.end_source(step);
               if (got != source) {
                 t.errorf("Source::Out: got {}, want {}", got, source);
               }
             }));

    add_test("track_end_source(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 5;
               module.set(Controls::Param::StepTracksEndSource, step, 1.F);
               auto track = controls.track_end_source(step);
               if (!track) {
                 t.error("with tracking enabled: got false, want true");
               }
               module.set(Controls::Param::StepTracksEndSource, step, 0.F);
               track = controls.track_end_source(step);
               if (track) {
                 t.error("with tracking disabled: got true, want false");
               }
             }));

    add_test("end_level(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 2;
               auto constexpr rotation = 0.22F;
               auto constexpr range = 0;
               module.set(Controls::Param::StepEndLevel, step, rotation);
               module.set(Controls::Param::LevelRange, range);
               auto want = dhe::level(rotation, dhe::signal_ranges[range]);
               auto got = controls.end_level(step);
               if (got != want) {
                 t.errorf("got {}, want {}", got, want);
               }
             }));

    add_test("duration(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 2;
               auto constexpr rotation = 0.22F;
               auto constexpr range = 0;
               module.set(Controls::Param::StepDuration, step, rotation);
               module.set(Controls::Param::DurationRange, range);
               auto want = dhe::duration(rotation, dhe::duration_ranges[range]);
               auto got = controls.duration(step);
               if (got != want) {
                 t.errorf("got {}, want {}", got, want);
               }
             }));

    add_test("taper(s)", test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 3;
               auto selection = 0; // J
               module.set(Controls::Param::StepShape, step,
                          static_cast<float>(selection));
               auto want = dhe::sigmoid::tapers[selection];
               auto got = controls.taper(step);
               if (got != want) {
                 t.errorf("J: got {}, want {}", got, want);
               }

               selection = 1; // S
               module.set(Controls::Param::StepShape, step,
                          static_cast<float>(selection));
               want = dhe::sigmoid::tapers[selection];
               got = controls.taper(step);
               if (got != want) {
                 t.errorf("S: got {}, want {}", got, want);
               }
             }));

    add_test("curvature(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 5;
               auto constexpr rotation = 0.68F;
               module.set(Controls::Param::StepCurvature, step, rotation);
               auto want = dhe::curvature(rotation);
               auto got = controls.curvature(step);
               if (got != want) {
                 t.errorf("got {}, want {}", got, want);
               }
             }));

    add_test("show_progress(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 6;

               auto constexpr completed_index = step + step;
               auto constexpr remaining_index = step + step + 1;

               controls.show_progress(step, 0.4F);

               auto const completed1 =
                   module.get(Controls::Light::StepProgress, completed_index);
               if (completed1 <= 0.F) {
                 t.errorf("got completed {}, want greater than 0", completed1);
               }

               auto const remaining1 =
                   module.get(Controls::Light::StepProgress, remaining_index);

               if (remaining1 >= 1.F) {
                 t.errorf("got remaining {}, want less than 1", remaining1);
               }

               controls.show_progress(step, 0.6F);

               auto const completed2 =
                   module.get(Controls::Light::StepProgress, completed_index);
               if (completed2 <= completed1) {
                 t.errorf("got completed {}, want greater than {}", completed2,
                          completed1);
               }

               auto const remaining2 =
                   module.get(Controls::Light::StepProgress, remaining_index);

               if (remaining2 >= remaining1) {
                 t.errorf("got remaining {}, want less than {}", remaining2,
                          remaining1);
               }
             }));

    add_test("show_inactive(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 5;

               auto constexpr completed_index = step + step;
               auto constexpr remaining_index = step + step + 1;

               controls.show_inactive(step);

               auto const completed =
                   module.get(Controls::Light::StepProgress, completed_index);
               if (completed != 0.F) {
                 t.errorf("got completed {}, want 0", completed);
               }

               auto const remaining =
                   module.get(Controls::Light::StepProgress, remaining_index);

               if (remaining != 0.F) {
                 t.errorf("got remaining {}, want 0", remaining);
               }
             }));

    add_test("is_enabled(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 3;

               module.set(Controls::Param::EnableStep, step, 1.F);
               module.set(Controls::Input::EnableStep, step, 0.F);
               auto enabled = controls.is_enabled(step);
               if (!enabled) {
                 t.errorf("button pressed: got {}, want {}", enabled, !enabled);
               }

               module.set(Controls::Param::EnableStep, step, 0.F);
               module.set(Controls::Input::EnableStep, step, 10.F);
               enabled = controls.is_enabled(step);
               if (!enabled) {
                 t.errorf("input high: got {}, want {}", enabled, !enabled);
               }

               module.set(Controls::Param::EnableStep, step, 0.F);
               module.set(Controls::Input::EnableStep, step, 0.F);
               enabled = controls.is_enabled(step);
               if (enabled) {
                 t.errorf("input high, button released: got {}, want {}",
                          enabled, !enabled);
               }
             }));
  }
};

__attribute__((unused)) static StepControlsTests step_controls_test{};
} // namespace step_controls
} // namespace curve_sequencer_2
} // namespace test
