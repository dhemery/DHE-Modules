#include "components/sigmoid.h"
#include "fake/rack-controls.h"
#include "modules/curve-sequencer-2/controls.h"
#include "modules/curve-sequencer-2/triggers.h"
#include <ostream>

#include <dheunit/test.h>

namespace dhe {
namespace curve_sequencer_2 {
static inline auto operator<<(std::ostream &os, TriggerMode mode)
    -> std::ostream & {
  os << static_cast<int>(mode);
  return os;
}
} // namespace curve_sequencer_2
} // namespace dhe

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
  auto param(int id) -> fake::Param & { return params_[id]; };
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

class StepControlsTests : Suite {
public:
  StepControlsTests() : Suite{"dhe::curve_sequencer_2::[Step]Controls"} {}
  void register_tests(dhe::unit::TestRegistrar add) override {
    add("trigger_mode(s)",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr step = 0;

          auto mode = TriggerMode::GateIsHigh;
          module.param(Controls::TriggerModeSwitches + step)
              .setValue(static_cast<float>(mode));
          TriggerMode got = controls.trigger_mode(step);
          if (got != mode) {
            t.errorf("GateIsHigh got mode {}, want {}", got, mode);
          }
        }));
  }
};

__attribute__((unused)) static StepControlsTests step_controls_test{};
} // namespace step_controls
} // namespace curve_sequencer_2
} // namespace test
