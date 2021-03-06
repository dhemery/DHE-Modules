#include "./fixtures/sequence-controller-fixture.h"
#include "./fixtures/status-enums.h"
#include "dheunit/assertions.h"
#include <helpers/latches.h>

namespace test {
namespace sequencizer {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

static auto constexpr initially_active_step = 1;
static inline void when_active(Module &module, StepSelector &step_selector,
                               StepController &step_controller,
                               SequenceController &sequence_controller) {

  module.running_ = true;
  module.reset_ = false;

  // Prepare to enter and execute some step
  module.gate_ = true;
  step_selector.first_ = initially_active_step;
  step_controller.status_ = StepStatus::Generating;

  // Enter and execute the prepared step
  sequence_controller.execute(0.F);

  // Reset the sequencer module
  module.running_ = false;
  module.gate_ = false;

  // Reset the step selector and step controller
  step_selector = StepSelector{};
  step_controller = StepController{};
}

class SequenceControllerActiveSuite : Suite {
public:
  SequenceControllerActiveSuite()
      : Suite{"dhe::sequencizer::SequenceController: active"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("with run high: "
        "executes active step with gate state and sample time",
        test(when_active, [](Tester &t, Module &module,
                             StepSelector & /*step_selector*/,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
          module.running_ = true;
          step_controller.status_ = StepStatus::Generating;
          auto constexpr sample_time{0.14901F};

          module.gate_ = true;
          sequence_controller.execute(sample_time);
          // Assume that the gate was high on the previous sample,
          // so on this sample it will be high with no edge.
          t.assert_that("high latch", step_controller.executed_latch_,
                        is_equal_to(high_latch));
          t.assert_that("sample time", step_controller.executed_sample_time_,
                        is_equal_to(sample_time));

          module.gate_ = false;
          sequence_controller.execute(0.1F);
          t.assert_that("falling latch", step_controller.executed_latch_,
                        is_equal_to(falling_latch));

          sequence_controller.execute(0.1F);
          t.assert_that("low latch", step_controller.executed_latch_,
                        is_equal_to(low_latch));

          module.gate_ = true;
          sequence_controller.execute(0.1F);
          t.assert_that("rising latch", step_controller.executed_latch_,
                        is_equal_to(rising_latch));
        }));

    add("with run high: "
        "shows step status",
        test(when_active,
             [](Tester &t, Module &module, StepSelector & /*step_selector*/,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = true;

               auto step_status = StepStatus::Generating;

               step_controller.status_ = step_status;
               sequence_controller.execute(0.F);
               t.assert_that("displayed step after generating", module.step_,
                             is_equal_to(initially_active_step));
               t.assert_that("status after generating", module.status_,
                             is_equal_to(step_status));

               step_status = StepStatus::Sustaining;

               step_controller.status_ = step_status;
               sequence_controller.execute(0.F);
               t.assert_that("displayed step after sustaining", module.step_,
                             is_equal_to(initially_active_step));
               t.assert_that("status after sustaining", module.status_,
                             is_equal_to(step_status));
             }));

    add("if active step completes: "
        "enters successor of active step",
        test(when_active,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = true;

               step_controller.status_ = StepStatus::Idle;
               auto constexpr successor = initially_active_step + 3;
               step_selector.successor_ = successor;

               sequence_controller.execute(0.F);
               t.assert_that("step given to selector", step_selector.step_,
                             is_equal_to(initially_active_step));
               t.assert_that("entered step", step_controller.entered_step_,
                             is_equal_to(successor));
               t.assert_that("step status step", module.step_,
                             is_equal_to(successor));
               t.assert_that("step status", module.status_,
                             is_equal_to(StepStatus::Generating));
             }));

    add("if active step completes: "
        "if no successor: "
        "if looping: "
        "restarts sequence",
        test(when_active, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
          module.running_ = true;
          module.looping_ = true;

          auto constexpr first = initially_active_step + 2;
          step_controller.status_ = StepStatus::Idle;
          step_selector.successor_ = -1;
          step_selector.first_ = first;

          sequence_controller.execute(0.F);
          t.assert_that("step status step", module.step_, is_equal_to(first));
          t.assert_that("step status", module.status_,
                        is_equal_to(StepStatus::Generating));
        }));

    add("if active step completes: "
        "if no successor: "
        "if not looping: "
        "stops",
        test(when_active,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = true;
               module.looping_ = false;

               step_controller.status_ = StepStatus::Idle;
               step_selector.successor_ = -1;

               sequence_controller.execute(0.F);
               t.assert_that("step controller entered",
                             step_controller.entered_, is_false);
               t.assert_that("step status step", module.step_, is_equal_to(-1));
               t.assert_that("step status", module.status_,
                             is_equal_to(StepStatus::Idle));
             }));

    add("with run low: "
        "with gate low: "
        "does nothing",
        test(when_active, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
          module.running_ = false;
          module.gate_ = false;

          auto constexpr original_output = 876.342F;
          module.output_ = original_output;

          sequence_controller.execute(0.F);
          t.assert_that("step selector called", step_selector.called_,
                        is_false);
          t.assert_that("step controller called", step_controller.called_,
                        is_false);
          t.assert_that("output", module.output_, is_equal_to(original_output));
        }));

    add("with run low: "
        "if gate rises: "
        "does nothing",
        test(when_active, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
          module.running_ = false;
          module.gate_ = true;

          auto constexpr original_output = 876.342F;
          module.output_ = original_output;

          sequence_controller.execute(0.F);
          t.assert_that("step selector called", step_selector.called_,
                        is_false);
          t.assert_that("step controller called", step_controller.called_,
                        is_false);
          t.assert_that("output", module.output_, is_equal_to(original_output));
        }));

    add("with run low: "
        "with reset low: "
        "does nothing",
        test(when_active, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
          module.running_ = false;
          module.reset_ = true;

          auto constexpr original_output = 876.342F;
          module.output_ = original_output;

          sequence_controller.execute(0.F);
          t.assert_that("step selector called", step_selector.called_,
                        is_false);
          t.assert_that("step controller called", step_controller.called_,
                        is_false);
          t.assert_that("output", module.output_, is_equal_to(original_output));
        }));

    add("with run low: "
        "if reset rises: "
        "exits active step and does not select or enter another",
        test(when_active, [](Tester &t, Module &module,
                             StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
          module.running_ = false;
          module.reset_ = true;

          auto constexpr original_output = 876.342F;
          module.output_ = original_output;

          sequence_controller.execute(0.F);
          t.assert_that("step selector called", step_selector.called_,
                        is_false);
          t.assert_that("step controller exited", step_controller.exited_,
                        is_true);
          t.assert_that("step controller entered", step_controller.entered_,
                        is_false);
          t.assert_that("output", module.output_, is_equal_to(original_output));
        }));
  }
};
__attribute__((unused)) static auto _ = SequenceControllerActiveSuite{};
} // namespace sequencizer
} // namespace test
