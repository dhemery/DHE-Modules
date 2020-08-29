#include "components/sigmoid.h"
#include "controls-test.h"
#include "modules/curve-sequencer-2/controls.h"
#include "modules/curve-sequencer-2/triggers.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer_2 {
namespace step_controls {
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
               module.set(Controls::Param::StepInterruptsOnTrigger, step, 0.F);
               auto interrupt_on_trigger = controls.interrupt_on_trigger(step);
               if (interrupt_on_trigger) {
                 t.errorf("with option disabled, got true, want false");
               }
               module.set(Controls::Param::StepInterruptsOnTrigger, step, 1.F);
               interrupt_on_trigger = controls.interrupt_on_trigger(step);
               if (!interrupt_on_trigger) {
                 t.errorf("with option enabled, got false, want true");
               }
             }));

    add_test("advance_on_end_of_curve(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 3;
               module.set(Controls::Param::StepAdvancesOnEndOfCurve, step, 0.F);
               auto advance = controls.advance_on_end_of_curve(step);
               if (advance) {
                 t.errorf("with option disabled, got true, want false");
               }
               module.set(Controls::Param::StepAdvancesOnEndOfCurve, step, 1.F);
               advance = controls.advance_on_end_of_curve(step);
               if (!advance) {
                 t.errorf("with option enabled, got false, want true");
               }
             }));
  }
};

__attribute__((unused)) static StepControlsTests step_controls_test{};
} // namespace step_controls
} // namespace curve_sequencer_2
} // namespace test
