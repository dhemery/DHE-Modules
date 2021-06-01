#include "./fixtures/controls-fixture.h"
#include <dheunit/test.h>
#include <functional>

namespace test {
namespace curve_sequencer {

using dhe::unit::Suite;
using dhe::unit::Tester;

struct InputButtonComboTest {
  std::string name_; // NOLINT
  float button_;     // NOLINT
  float input_;      // NOLINT
  bool want_;        // NOLINT

  void run(Tester &t, std::string const &func_name, int button_id, int input,
           std::function<bool(Controls &)> const &func) const {
    t.run(func_name + ": " + name_, [this, button_id, input, func](Tester &t) {
      auto module = Module{};
      auto controls = Controls{module.inputs_, module.outputs_, module.params_,
                               module.lights_};
      module.params_[button_id].setValue(button_);
      module.inputs_[input].setVoltage(input_);

      auto const got = func(controls);
      if (got != want_) {
        t.errorf("got {}, want {}", got, want_);
      }
    });
  }
};

std::vector<InputButtonComboTest> input_button_combo_tests = {
    {
        .name_ = "button up, input low",
        .button_ = 0.F,
        .input_ = 0.F,
        .want_ = false,
    },
    {
        .name_ = "button down, input low",
        .button_ = 1.F,
        .input_ = 0.F,
        .want_ = true,
    },
    {
        .name_ = "button up, input high",
        .button_ = 0.F,
        .input_ = 10.F,
        .want_ = true,
    },
    {
        .name_ = "button down, input high",
        .button_ = 1.F,
        .input_ = 10.F,
        .want_ = true,
    },
};

class ControlsSuite : public Suite {
public:
  ControlsSuite() : Suite("dhe::curve_sequencer::Controls") {}
  void run(Tester &t) override {
    for (auto &test : input_button_combo_tests) {
      test.run(t, "is_gated()", Param::GateButton, Input::GateInput,
               [](Controls &controls) -> bool { return controls.is_gated(); });
    }

    for (auto &test : input_button_combo_tests) {
      test.run(
          t, "is_looping()", Param::LoopButton, Input::LoopInput,
          [](Controls &controls) -> bool { return controls.is_looping(); });
    }

    for (auto &test : input_button_combo_tests) {
      test.run(t, "is_reset()", Param::ResetButton, Input::ResetInput,
               [](Controls &controls) -> bool { return controls.is_reset(); });
    }

    for (auto &test : input_button_combo_tests) {
      test.run(
          t, "is_running()", Param::RunButton, Input::RunInput,
          [](Controls &controls) -> bool { return controls.is_running(); });
    }

    for (auto &test : input_button_combo_tests) {
      auto const step = std::rand() % step_count;
      test.run(t, "is_enabled()", Param::EnabledButtons + step,
               Input::EnabledInputs + step, [step](Controls &controls) -> bool {
                 return controls.is_enabled(step);
               });
    }

    t.run("selection_start()",
          test([](Tester &t, Module &module, Controls &controls) {
            auto const want = std::rand() % step_count;
            auto const knob_value = static_cast<float>(want);
            module.params_[Param::SelectionStartKnob].setValue(knob_value);

            auto const got = controls.selection_start();
            if (got != want) {
              t.errorf("Got {}, want {}");
            }
          }));

    t.run("selection_length()",
          test([](Tester &t, Module &module, Controls &controls) {
            auto const want = std::rand() % step_count;
            auto const knob_value = static_cast<float>(want);
            module.params_[Param::SelectionLengthKnob].setValue(knob_value);
            auto const got = controls.selection_length();
            if (got != want) {
              t.errorf("Got {}, want {}", got, want);
            }
          }));

    t.run("input()", test([](Tester &t, Module &module, Controls &controls) {
            auto constexpr input_voltage = 3.1234F;
            module.inputs_[Input::CurveSequencerInput].setVoltage(
                input_voltage);

            auto const got = controls.input();
            if (got != input_voltage) {
              t.errorf("Got {}, want {}", got, input_voltage);
            }
          }));

    t.run("output()", test([](Tester &t, Module &module, Controls &controls) {
            auto constexpr output_voltage = 9.13894F;
            module.outputs_[Output::CurveSequencerOutput].setVoltage(
                output_voltage);

            auto const got = controls.output();
            if (got != output_voltage) {
              t.errorf("Got {}, want {}", got, output_voltage);
            }
          }));

    t.run("output(v)", test([](Tester &t, Module &module, Controls &controls) {
            auto constexpr output_voltage = 4.390984F;
            controls.output(output_voltage);

            auto const got =
                module.outputs_[Output::CurveSequencerOutput].getVoltage();

            if (got != output_voltage) {
              t.errorf("Got {}, want {}", got, output_voltage);
            }
          }));
  }
};

static auto _ = ControlsSuite{};
} // namespace curve_sequencer
} // namespace test
