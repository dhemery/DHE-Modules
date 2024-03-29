#include "fixtures/sequence-controller-fixture.h"
#include "fixtures/status-enums.h"

#include "components/latch.h"
#include "helpers/assertions.h"

#include <functional>

namespace test {
namespace sequencizer {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_false;
using test::is_true;
using TestFunc = std::function<void(Tester &)>;

static auto constexpr initially_active_step = 1;
static inline void when_active(Signals &signals, StepSelector &step_selector,
                               StepController &step_controller,
                               SequenceController &sequence_controller) {

  signals.running_ = true;
  signals.reset_ = false;

  // Prepare to enter and execute some step
  signals.gate_ = true;
  step_selector.first_ = initially_active_step;
  step_controller.status_ = StepStatus::Generating;

  // Enter and execute the prepared step
  sequence_controller.execute(0.F);

  // Reset the sequencer signals
  signals.running_ = false;
  signals.gate_ = false;

  // Reset the step selector and step controller
  step_selector = StepSelector{};
  step_controller = StepController{};
}

class SequenceControllerActiveSuite : Suite {
public:
  SequenceControllerActiveSuite()
      : Suite{"dhe::sequencizer::SequenceController: active"} {}

  void run(Tester &t) override {
    t.run("with run high: "
          "executes active step with gate state and sample time",
          test(when_active, [](Tester &t, Signals &signals,
                               StepSelector & /*step_selector*/,
                               StepController &step_controller,
                               SequenceController &sequence_controller) {
            signals.running_ = true;
            step_controller.status_ = StepStatus::Generating;
            auto constexpr sample_time{0.14901F};

            signals.gate_ = true;
            sequence_controller.execute(sample_time);
            // Assume that the gate was high on the previous sample,
            // so on this sample it will be high with no edge.
            assert_that(t, "high latch", step_controller.executed_latch_,
                        is_equal_to(dhe::latch::high));
            assert_that(t, "sample time", step_controller.executed_sample_time_,
                        is_equal_to(sample_time));

            signals.gate_ = false;
            sequence_controller.execute(0.1F);
            assert_that(t, "falling latch", step_controller.executed_latch_,
                        is_equal_to(dhe::latch::falling));

            sequence_controller.execute(0.1F);
            assert_that(t, "low latch", step_controller.executed_latch_,
                        is_equal_to(dhe::latch::low));

            signals.gate_ = true;
            sequence_controller.execute(0.1F);
            assert_that(t, "rising latch", step_controller.executed_latch_,
                        is_equal_to(dhe::latch::rising));
          }));

    t.run("with run high: "
          "shows step status",
          test(when_active,
               [](Tester &t, Signals &signals, StepSelector & /*step_selector*/,
                  StepController &step_controller,
                  SequenceController &sequence_controller) {
                 signals.running_ = true;

                 auto step_status = StepStatus::Generating;

                 step_controller.status_ = step_status;
                 sequence_controller.execute(0.F);
                 assert_that(t, "displayed step after generating",
                             signals.step_, is_equal_to(initially_active_step));
                 assert_that(t, "status after generating", signals.status_,
                             is_equal_to(step_status));

                 step_status = StepStatus::Sustaining;

                 step_controller.status_ = step_status;
                 sequence_controller.execute(0.F);
                 assert_that(t, "displayed step after sustaining",
                             signals.step_, is_equal_to(initially_active_step));
                 assert_that(t, "status after sustaining", signals.status_,
                             is_equal_to(step_status));
               }));

    t.run("if active step completes: "
          "enters successor of active step",
          test(when_active,
               [](Tester &t, Signals &signals, StepSelector &step_selector,
                  StepController &step_controller,
                  SequenceController &sequence_controller) {
                 signals.running_ = true;

                 step_controller.status_ = StepStatus::Idle;
                 auto constexpr successor = initially_active_step + 3;
                 step_selector.successor_ = successor;

                 sequence_controller.execute(0.F);
                 assert_that(t, "step given to selector", step_selector.step_,
                             is_equal_to(initially_active_step));
                 assert_that(t, "entered step", step_controller.entered_step_,
                             is_equal_to(successor));
                 assert_that(t, "step status step", signals.step_,
                             is_equal_to(successor));
                 assert_that(t, "step status", signals.status_,
                             is_equal_to(StepStatus::Generating));
               }));

    t.run("if active step completes: "
          "if no successor: "
          "if looping: "
          "restarts sequence",
          test(when_active,
               [](Tester &t, Signals &signals, StepSelector &step_selector,
                  StepController &step_controller,
                  SequenceController &sequence_controller) {
                 signals.running_ = true;
                 signals.looping_ = true;

                 auto constexpr first = initially_active_step + 2;
                 step_controller.status_ = StepStatus::Idle;
                 step_selector.successor_ = -1;
                 step_selector.first_ = first;

                 sequence_controller.execute(0.F);
                 assert_that(t, "step status step", signals.step_,
                             is_equal_to(first));
                 assert_that(t, "step status", signals.status_,
                             is_equal_to(StepStatus::Generating));
               }));

    t.run("if active step completes: "
          "if no successor: "
          "if not looping: "
          "stops",
          test(when_active, [](Tester &t, Signals &signals,
                               StepSelector &step_selector,
                               StepController &step_controller,
                               SequenceController &sequence_controller) {
            signals.running_ = true;
            signals.looping_ = false;

            step_controller.status_ = StepStatus::Idle;
            step_selector.successor_ = -1;

            sequence_controller.execute(0.F);
            assert_that(t, "step controller entered", step_controller.entered_,
                        is_false);
            assert_that(t, "step status step", signals.step_, is_equal_to(-1));
            assert_that(t, "step status", signals.status_,
                        is_equal_to(StepStatus::Idle));
          }));

    t.run("with run low: "
          "with gate low: "
          "does nothing",
          test(when_active,
               [](Tester &t, Signals &signals, StepSelector &step_selector,
                  StepController &step_controller,
                  SequenceController &sequence_controller) {
                 signals.running_ = false;
                 signals.gate_ = false;

                 auto constexpr original_output = 876.342F;
                 signals.output_ = original_output;

                 sequence_controller.execute(0.F);
                 assert_that(t, "step selector called", step_selector.called_,
                             is_false);
                 assert_that(t, "step controller called",
                             step_controller.called_, is_false);
                 assert_that(t, "output", signals.output_,
                             is_equal_to(original_output));
               }));

    t.run("with run low: "
          "if gate rises: "
          "does nothing",
          test(when_active,
               [](Tester &t, Signals &signals, StepSelector &step_selector,
                  StepController &step_controller,
                  SequenceController &sequence_controller) {
                 signals.running_ = false;
                 signals.gate_ = true;

                 auto constexpr original_output = 876.342F;
                 signals.output_ = original_output;

                 sequence_controller.execute(0.F);
                 assert_that(t, "step selector called", step_selector.called_,
                             is_false);
                 assert_that(t, "step controller called",
                             step_controller.called_, is_false);
                 assert_that(t, "output", signals.output_,
                             is_equal_to(original_output));
               }));

    t.run("with run low: "
          "with reset low: "
          "does nothing",
          test(when_active,
               [](Tester &t, Signals &signals, StepSelector &step_selector,
                  StepController &step_controller,
                  SequenceController &sequence_controller) {
                 signals.running_ = false;
                 signals.reset_ = true;

                 auto constexpr original_output = 876.342F;
                 signals.output_ = original_output;

                 sequence_controller.execute(0.F);
                 assert_that(t, "step selector called", step_selector.called_,
                             is_false);
                 assert_that(t, "step controller called",
                             step_controller.called_, is_false);
                 assert_that(t, "output", signals.output_,
                             is_equal_to(original_output));
               }));

    t.run("with run low: "
          "if reset rises: "
          "exits active step and does not select or enter another",
          test(when_active,
               [](Tester &t, Signals &signals, StepSelector &step_selector,
                  StepController &step_controller,
                  SequenceController &sequence_controller) {
                 signals.running_ = false;
                 signals.reset_ = true;

                 auto constexpr original_output = 876.342F;
                 signals.output_ = original_output;

                 sequence_controller.execute(0.F);
                 assert_that(t, "step selector called", step_selector.called_,
                             is_false);
                 assert_that(t, "step controller exited",
                             step_controller.exited_, is_true);
                 assert_that(t, "step controller entered",
                             step_controller.entered_, is_false);
                 assert_that(t, "output", signals.output_,
                             is_equal_to(original_output));
               }));
  }
};

static auto _ = SequenceControllerActiveSuite{};
} // namespace sequencizer
} // namespace test
