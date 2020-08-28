#include "components/sigmoid.h"
#include "helpers/rack-controls.h"
#include "modules/curve-sequencer-2/controls.h"
#include "modules/curve-sequencer-2/triggers.h"

#include "trigger-helpers.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer_2 {
namespace step_controls {
static auto constexpr step_count{8};
using Controls = dhe::curve_sequencer_2::Controls<fake::Port, fake::Param,
                                                  fake::Light, step_count>;
using dhe::curve_sequencer_2::Source;
using dhe::curve_sequencer_2::TriggerMode;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class Module {
public:
  auto inputs() -> std::vector<fake::Port> & { return inputs_; };
  auto outputs() -> std::vector<fake::Port> & { return outputs_; };
  auto params() -> std::vector<fake::Param> & { return params_; };
  auto lights() -> std::vector<fake::Light> & { return lights_; };
  auto input(Controls::InputIds id) -> fake::Port & { return inputs_[id]; };
  auto output(Controls::OutputIds id) -> fake::Port & { return outputs_[id]; };
  auto param(Controls::ParamIds id, int offset = 0) -> fake::Param & {
    return params_[id + offset];
  };
  auto light(Controls::LightIds id) -> fake::Light & { return lights_[id]; };

private:
  std::vector<fake::Port> inputs_{Controls::InputCount};
  std::vector<fake::Port> outputs_{Controls::OutputCount};
  std::vector<fake::Param> params_{Controls::ParamCount};
  std::vector<fake::Light> lights_{Controls::LightCount};
};

auto test(const std::function<void(Tester &, Module &, Controls &)> &run)
    -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Controls controls{module.inputs(), module.outputs(), module.params(),
                      module.lights()};
    run(t, module, controls);
  };
}

auto trigger_mode_test(TriggerMode mode) -> TestFunc {
  return test([mode](Tester &t, Module &module, Controls &controls) {
    auto constexpr step = 4;
    auto const switch_value = static_cast<float>(mode);
    module.param(Controls::TriggerModeSwitches, step).setValue(switch_value);
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
    for (auto mode : test::curve_sequencer_2::TriggerModeIterator()) {
      auto const name = std::string{"trigger_mode(s): "} + name_of(mode);
      add_test(name, trigger_mode_test(mode));
    }

    add_test("interrupt_on_trigger(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 3;
               module.param(Controls::OnInterruptSwitches, step).setValue(0.F);
               auto interrupt_on_trigger = controls.interrupt_on_trigger(step);
               if (interrupt_on_trigger) {
                 t.errorf("with option disabled, got true, want false");
               }
               module.param(Controls::OnInterruptSwitches, step).setValue(1.F);
               interrupt_on_trigger = controls.interrupt_on_trigger(step);
               if (!interrupt_on_trigger) {
                 t.errorf("with option enabled, got false, want true");
               }
             }));

    add_test("advance_on_end_of_curve(s)",
             test([](Tester &t, Module &module, Controls controls) {
               auto constexpr step = 3;
               module.param(Controls::OnEndOfCurveSwitches, step).setValue(0.F);
               auto advance = controls.advance_on_end_of_curve(step);
               if (advance) {
                 t.errorf("with option disabled, got true, want false");
               }
               module.param(Controls::OnEndOfCurveSwitches, step).setValue(1.F);
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
