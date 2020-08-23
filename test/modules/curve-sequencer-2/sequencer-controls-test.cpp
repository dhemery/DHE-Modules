#include "fake/rack-controls.h"
#include "modules/curve-sequencer-2/controls.h"

#include "dheunit/test.h"
#include <functional>

namespace test {
namespace curve_sequencer_2 {
namespace sequence_controls {
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

static auto constexpr step_count{8};

using Controls = dhe::curve_sequencer_2::Controls<fake::Port, fake::Param,
                                                  fake::Light, step_count>;

class Module {
public:
  auto inputs() -> std::vector<fake::Port> & { return inputs_; };
  auto outputs() -> std::vector<fake::Port> & { return outputs_; };
  auto params() -> std::vector<fake::Param> & { return params_; };
  auto lights() -> std::vector<fake::Light> & { return lights_; };
  auto input(Controls::InputIds id) -> fake::Port & { return inputs_[id]; };
  auto output(Controls::OutputIds id) -> fake::Port & { return outputs_[id]; };
  auto param(Controls::ParamIds id) -> fake::Param & { return params_[id]; };
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

class ControlsGatedTests : Suite {
public:
  ControlsGatedTests() : Suite{"dhe::curve_sequencer_2::Controls"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("is_gated()", test([](Tester &t, Module &module, Controls &controls) {
          module.param(Controls::GateButton).setValue(1.F);
          module.input(Controls::GateInput).setVoltage(0.F);
          if (!controls.is_gated()) {
            t.error("button pressed: got false, want true");
          }

          module.param(Controls::GateButton).setValue(0.F);
          module.input(Controls::GateInput).setVoltage(10.F);
          if (!controls.is_gated()) {
            t.error("input high: got false, want true");
          }

          module.param(Controls::GateButton).setValue(0.F);
          module.input(Controls::GateInput).setVoltage(0.F);
          if (controls.is_gated()) {
            t.error("input low and button released: got false, want true");
          }
        }));

    add("is_looping()", test([](Tester &t, Module &module, Controls &controls) {
          module.param(Controls::LoopButton).setValue(1.F);
          module.input(Controls::LoopInput).setVoltage(0.F);
          if (!controls.is_looping()) {
            t.error("button pressed: got false, want true");
          }

          module.param(Controls::LoopButton).setValue(0.F);
          module.input(Controls::LoopInput).setVoltage(10.F);
          if (!controls.is_looping()) {
            t.error("input high: got false, want true");
          }

          module.param(Controls::LoopButton).setValue(0.F);
          module.input(Controls::LoopInput).setVoltage(0.F);
          if (controls.is_looping()) {
            t.error("input low and button not pressed: got false, want true");
          }
        }));

    add("is_reset()", test([](Tester &t, Module &module, Controls &controls) {
          module.param(Controls::ResetButton).setValue(1.F);
          module.input(Controls::ResetInput).setVoltage(0.F);
          if (!controls.is_reset()) {
            t.error("button pressed: got false, want true");
          }

          module.param(Controls::ResetButton).setValue(0.F);
          module.input(Controls::ResetInput).setVoltage(10.F);
          if (!controls.is_reset()) {
            t.error("input high: got false, want true");
          }

          module.param(Controls::ResetButton).setValue(0.F);
          module.input(Controls::ResetInput).setVoltage(0.F);
          if (controls.is_reset()) {
            t.error("input low and button not pressed: got false, want true");
          }
        }));

    add("is_running()", test([](Tester &t, Module &module, Controls &controls) {
          module.param(Controls::RunButton).setValue(1.F);
          module.input(Controls::RunInput).setVoltage(0.F);
          if (!controls.is_running()) {
            t.error("button pressed: got false, want true");
          }

          module.param(Controls::RunButton).setValue(0.F);
          module.input(Controls::RunInput).setVoltage(10.F);
          if (!controls.is_running()) {
            t.error("input high: got false, want true");
          }

          module.param(Controls::RunButton).setValue(0.F);
          module.input(Controls::RunInput).setVoltage(0.F);
          if (controls.is_running()) {
            t.error("input low and button not pressed: got true, want false");
          }
        }));

    add("selection_start()",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr start = 2;
          module.param(Controls::SelectionStartKnob).setValue(start);

          int got = controls.selection_start();
          if (got != start) {
            t.error("got {}, want {}", got, start);
          }
        }));

    add("selection_length()",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr length = 5;
          module.param(Controls::SelectionLengthKnob).setValue(length);

          int got = controls.selection_length();
          if (got != length) {
            t.error("got {}, want {}", got, length);
          }
        }));

    add("input()", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr input = 7.777F;
          module.input(Controls::CurveSequencerInput).setVoltage(input);

          float got = controls.input();
          if (got != input) {
            t.error("got {}, want {}", got, input);
          }
        }));

    add("output()", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr output = 3.333F;
          module.output(Controls::CurveSequencerOutput).setVoltage(output);

          float got = controls.output();
          if (got != output) {
            t.error("got {}, want {}", got, output);
          }
        }));

    add("output(v)", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr output = 4.444F;

          controls.output(output);

          auto const got =
              module.output(Controls::CurveSequencerOutput).getVoltage();
          if (got != output) {
            t.error("got {}, want {}", got, output);
          }
        }));
  }
};

__attribute__((unused)) static ControlsGatedTests controls_gated_tests{};

} // namespace sequence_controls
} // namespace curve_sequencer_2
} // namespace test
