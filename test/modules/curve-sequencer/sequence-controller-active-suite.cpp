#include "./fixtures/sequence-controller-fixture.h"
#include "dheunit/assertions.h"
#include <helpers/latches.h>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

static auto constexpr initially_active_step = 1;
static inline void when_active(Controls &controls, StepSelector &step_selector,
                               StepController &step_controller,
                               SequenceController &sequence_controller) {

  controls.running_ = true;
  controls.reset_ = false;

  // Prepare to enter and execute some step
  controls.gate_ = true;
  step_selector.first_ = initially_active_step;
  step_controller.status_ = StepStatus::InProgress;

  // Enter and execute the prepared step
  sequence_controller.execute(0.F);

  // Reset the sequencer controls
  controls.running_ = false;
  controls.gate_ = false;

  // Reset the step selector and step controller
  step_selector = StepSelector{};
  step_controller = StepController{};
}

class SequenceControllerActiveSuite : Suite {
public:
  SequenceControllerActiveSuite()
      : Suite{"dhe::curve_sequencer::SequenceController: active"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("with run high: "
        "executes active step with gate state and sample time",
        test(when_active, [](Tester &t, Controls &controls,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
          controls.running_ = true;
          step_controller.status_ = StepStatus::InProgress;
          auto constexpr sample_time{0.14901F};

          controls.gate_ = true;
          sequence_controller.execute(sample_time);
          // Assume that the gate was high on the previous sample,
          // so on this sample it will be high with no edge.
          t.assert_that("high latch", step_controller.executed_latch_,
                        is_equal_to(high_latch));
          t.assert_that("sample time", step_controller.executed_sample_time_,
                        is_equal_to(sample_time));

          controls.gate_ = false;
          sequence_controller.execute(0.1F);
          t.assert_that("falling latch", step_controller.executed_latch_,
                        is_equal_to(falling_latch));

          sequence_controller.execute(0.1F);
          t.assert_that("low latch", step_controller.executed_latch_,
                        is_equal_to(low_latch));

          controls.gate_ = true;
          sequence_controller.execute(0.1F);
          t.assert_that("rising latch", step_controller.executed_latch_,
                        is_equal_to(rising_latch));
        }));

    add("with run high: "
        "if active step completes: "
        "seeks successor with active step and loop state",
        test(when_active,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               auto constexpr looping = true;
               controls.running_ = true;
               controls.looping_ = looping;
               step_controller.status_ = StepStatus::Completed;

               sequence_controller.execute(0.F);
               t.assert_that("step", step_selector.step_,
                             is_equal_to(initially_active_step));
               t.assert_that("looping", step_selector.looping_,
                             is_equal_to(looping));
             }));

    add("with run high: "
        "if active step completes: "
        "enters successor step",
        test(when_active, [](Tester &t, Controls &controls,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
          controls.running_ = true;

          step_controller.status_ = StepStatus::Completed;
          auto constexpr second_step = initially_active_step + 3;
          step_selector.successor_ = second_step;

          sequence_controller.execute(0.F);
          t.assert_that("step entered when initial step completed",
                        step_controller.entered_step_,
                        is_equal_to(second_step));

          auto constexpr third_step = second_step + 3;
          step_selector.successor_ = third_step;
          step_controller.status_ = StepStatus::Completed;

          sequence_controller.execute(0.F);
          t.assert_that("step entered when second step completed",
                        step_controller.entered_step_, is_equal_to(third_step));
        }));

    add("with run high: "
        "if active step completes: "
        "if no successor: "
        "does not enter another step",
        test(when_active,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = true;

               step_controller.status_ = StepStatus::Completed;
               step_selector.successor_ = -1;

               sequence_controller.execute(0.F);
               t.assert_that("step controller entered",
                             step_controller.entered_, is_false);
             }));

    add("with run high: "
        "if reset rises: "
        "copies input to output, "
        "exits active step, "
        "and does not select or enter another",
        test(when_active,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = true;
               controls.reset_ = true;

               auto constexpr input = 3478.233F;
               controls.input_ = input;

               sequence_controller.execute(0.F);
               t.assert_that("output", controls.output_, is_equal_to(input));
               t.assert_that("step controller exited", step_controller.exited_,
                             is_true);
               t.assert_that("step selector called", step_selector.called_,
                             is_false);
               t.assert_that("step controller entered",
                             step_controller.entered_, is_false);
             }));

    add("with run low: "
        "with gate low: "
        "does nothing",
        test(when_active,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = false;
               controls.gate_ = false;

               auto constexpr original_output = 876.342F;
               controls.output_ = original_output;

               sequence_controller.execute(0.F);
               t.assert_that("step selector called", step_selector.called_,
                             is_false);
               t.assert_that("step controller called", step_controller.called_,
                             is_false);
               t.assert_that("output", controls.output_,
                             is_equal_to(original_output));
             }));

    add("with run low: "
        "if gate rises: "
        "does nothing",
        test(when_active,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = false;
               controls.gate_ = true;

               auto constexpr original_output = 876.342F;
               controls.output_ = original_output;

               sequence_controller.execute(0.F);
               t.assert_that("step selector called", step_selector.called_,
                             is_false);
               t.assert_that("step controller called", step_controller.called_,
                             is_false);
               t.assert_that("output", controls.output_,
                             is_equal_to(original_output));
             }));

    add("with run low: "
        "with reset low: "
        "does nothing",
        test(when_active,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = false;
               controls.reset_ = true;

               auto constexpr original_output = 876.342F;
               controls.output_ = original_output;

               sequence_controller.execute(0.F);
               t.assert_that("step selector called", step_selector.called_,
                             is_false);
               t.assert_that("step controller called", step_controller.called_,
                             is_false);
               t.assert_that("output", controls.output_,
                             is_equal_to(original_output));
             }));

    add("with run low: "
        "if reset rises: "
        "exits active step and does not select or enter another",
        test(when_active,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = false;
               controls.reset_ = true;

               auto constexpr original_output = 876.342F;
               controls.output_ = original_output;

               sequence_controller.execute(0.F);
               t.assert_that("step selector called", step_selector.called_,
                             is_false);
               t.assert_that("step controller exited", step_controller.exited_,
                             is_true);
               t.assert_that("step controller entered",
                             step_controller.entered_, is_false);
               t.assert_that("output", controls.output_,
                             is_equal_to(original_output));
             }));
  }
};
__attribute__((unused)) static auto _ = SequenceControllerActiveSuite{};
} // namespace curve_sequencer
} // namespace test
