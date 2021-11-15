#include "dheunit/test.h"
#include "fixtures/advance-mode-enums.h"
#include "fixtures/generate-mode-enums.h"
#include "fixtures/signals-fixture.h"

#include <functional>

namespace test {
namespace curve_sequencer {
using dhe::DurationRangeId;
using dhe::DurationRanges;
using dhe::unit::Suite;
using dhe::unit::Tester;

struct InputButtonComboTest {
  std::string name_; // NOLINT
  float button_;     // NOLINT
  float input_;      // NOLINT
  bool want_;        // NOLINT

  void run(Tester &t, std::string const &func_name, int button_id, int input,
           std::function<bool(Signals &)> const &func) const {
    t.run(func_name + ": " + name_, [this, button_id, input, func](Tester &t) {
      auto module = Module{};
      auto signals = Signals{module.params_, module.inputs_, module.outputs_,
                             module.lights_};
      module.params_[button_id].setValue(button_);
      module.inputs_[input].setVoltage(input_);

      auto const got = func(signals);
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

class SignalsSuite : public Suite {
public:
  SignalsSuite() : Suite("dhe::curve_sequencer::Signals") {}
  void run(Tester &t) override {
    for (auto &test : input_button_combo_tests) {
      test.run(t, "is_gated()", ParamId::Gate, InputId::Gate,
               [](Signals &signals) -> bool { return signals.is_gated(); });
    }

    for (auto &test : input_button_combo_tests) {
      test.run(t, "is_looping()", ParamId::Loop, InputId::Loop,
               [](Signals &signals) -> bool { return signals.is_looping(); });
    }
    for (auto &test : input_button_combo_tests) {
      test.run(t, "is_reset()", ParamId::Reset, InputId::Reset,
               [](Signals &signals) -> bool { return signals.is_reset(); });
    }

    for (auto &test : input_button_combo_tests) {
      test.run(t, "is_running()", ParamId::Run, InputId::Run,
               [](Signals &signals) -> bool { return signals.is_running(); });
    }

    for (auto &test : input_button_combo_tests) {
      auto const step = std::rand() % step_count;
      test.run(t, "is_enabled(step)", ParamId::StepEnabled + step,
               InputId::StepEnabled + step, [step](Signals &signals) -> bool {
                 return signals.is_enabled(step);
               });
    }

    t.run("selection_start()",
          test([](Tester &t, Module &module, Signals &signals) {
            auto const want = std::rand() % step_count;
            auto const knob_value = static_cast<float>(want);
            module.params_[ParamId::SelectionStart].setValue(knob_value);

            auto const got = signals.selection_start();
            if (got != want) {
              t.errorf("Got {}, want {}", got, want);
            }
          }));

    t.run("selection_length()",
          test([](Tester &t, Module &module, Signals &signals) {
            auto const want = std::rand() % step_count;
            auto const knob_value = static_cast<float>(want);
            module.params_[ParamId::SelectionLength].setValue(knob_value);
            auto const got = signals.selection_length();
            if (got != want) {
              t.errorf("Got {}, want {}", got, want);
            }
          }));

    t.run("input()", test([](Tester &t, Module &module, Signals &signals) {
            auto constexpr input_voltage = 3.1234F;
            module.inputs_[InputId::Main].setVoltage(input_voltage);

            auto const got = signals.input();
            if (got != input_voltage) {
              t.errorf("Got {}, want {}", got, input_voltage);
            }
          }));

    t.run("output()", test([](Tester &t, Module &module, Signals &signals) {
            auto constexpr output_voltage = 9.13894F;
            module.outputs_[OutputId::Main].setVoltage(output_voltage);

            auto const got = signals.output();
            if (got != output_voltage) {
              t.errorf("Got {}, want {}", got, output_voltage);
            }
          }));

    t.run("output(v)", test([](Tester &t, Module &module, Signals &signals) {
            auto constexpr output_voltage = 4.390984F;
            signals.output(output_voltage);

            auto const got = module.outputs_[OutputId::Main].getVoltage();

            if (got != output_voltage) {
              t.errorf("Got {}, want {}", got, output_voltage);
            }
          }));

    t.run("curvature(step) reports curvature for step curvature param",
          test([](Tester &t, Module &module, Signals &signals) {
            auto const step = std::rand() % step_count;
            auto constexpr curve_knob_rotation = 0.3F;

            module.params_[ParamId::StepCurvature + step].setValue(
                curve_knob_rotation);

            auto const got = signals.curvature(step);
            auto constexpr want = dhe::Curvature::scale(curve_knob_rotation);
            if (got != want) {
              t.errorf("Got {}, want {}", got, want);
            }
          }));

    t.run("duration(step) reports duration for duration select and step "
          "duration param",
          test([](Tester &t, Module &module, Signals &signals) {
            auto const step = std::rand() % step_count;
            auto constexpr duration_knob_rotation = 0.75F;
            auto constexpr duration_range_id =
                DurationRangeId::Long; // Long duration

            module.params_[ParamId::StepDuration + step].setValue(
                duration_knob_rotation);
            module.params_[ParamId::DurationRange].setValue(
                static_cast<float>(duration_range_id));

            auto const got = signals.duration(step);
            auto const want = DurationRanges::scale(duration_knob_rotation,
                                                    duration_range_id);
            if (got != want) {
              t.errorf("Got {}, want {}", got, want);
            }
          }));

    t.run("taper(step)", [](Tester &t) {
      for (unsigned long selection = 0; selection < dhe::sigmoid::tapers.size();
           selection++) {
        t.run("with switch in position " + std::to_string(selection),
              test([selection](Tester &t, Module &module, Signals &signals) {
                auto const step = std::rand() % step_count;
                module.params_[ParamId::StepShape + step].setValue(
                    static_cast<float>(selection));

                auto const got = signals.taper(step);
                auto const want = dhe::sigmoid::tapers[selection];

                if (got != want) {
                  t.errorf("Got {}, want {}", got, want);
                }
              }));
      }
    });

    t.run("generate_mode(step)", [](Tester &t) {
      using dhe::curve_sequencer::GenerateMode;
      using dhe::curve_sequencer::generate_mode_count;

      for (auto const mode : generate_modes) {
        t.run(name_of(mode),
              test([mode](Tester &t, Module &module, Signals &signals) {
                auto const step = std::rand() & step_count;
                module.params_[ParamId::StepGenerateMode + step].setValue(
                    static_cast<float>(mode));

                auto const got = signals.generate_mode(step);

                if (got != mode) {
                  t.errorf("Got {}, want {}", got, mode);
                }
              }));
      }
    });

    t.run("advance_mode(step)", [](Tester &t) {
      using dhe::curve_sequencer::AdvanceMode;
      using dhe::curve_sequencer::advance_mode_count;
      for (auto const mode : advance_modes) {
        t.run(name_of(mode),
              test([mode](Tester &t, Module &module, Signals &signals) {
                auto const step = std::rand() & step_count;
                module.params_[ParamId::StepAdvanceMode + step].setValue(
                    static_cast<float>(mode));

                auto const got = signals.advance_mode(step);

                if (got != mode) {
                  t.errorf("Got {}, want {}", got, mode);
                }
              }));
      }
    });

    t.run("show_inactive(step) dims progress lights_",
          test([](Tester &t, Module &module, Signals &signals) {
            auto const step = std::rand() % step_count;
            auto const completed_light_index =
                LightId::StepProgress + step + step;
            auto const remaining_light_index = completed_light_index + 1;

            signals.show_inactive(step);

            auto const completed_brightness =
                module.lights_[completed_light_index].getBrightness();
            if (completed_brightness != 0.F) {
              t.errorf("Got completed brightness {}, want 0",
                       completed_brightness);
            }
            auto const remaining_brightness =
                module.lights_[remaining_light_index].getBrightness();
            if (remaining_brightness != 0.F) {
              t.errorf("Got completed brightness {}, want 0",
                       remaining_brightness);
            }
          }));
  }
};

static auto _ = SignalsSuite{};
} // namespace curve_sequencer
} // namespace test
