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

static inline void when_idle(Controls &controls, StepSelector &step_selector,
                             StepController &step_controller,
                             SequenceController &sequence_controller) {
  controls.running_ = false;
  controls.reset_ = false;
  sequence_controller.execute(0.F);
}

class SequenceControllerIdleSuite : Suite {
public:
  SequenceControllerIdleSuite()
      : Suite{"dhe::curve_sequencer::SequenceController: idle"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("with run high: with reset high: copies input to output",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = true;
               controls.reset_ = true;

               auto constexpr input = 0.12394F;
               controls.input_ = input;

               sequence_controller.execute(0.1F);

               t.assert_that(controls.output_, is_equal_to(input));
               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
             }));

    add("with run high: with reset low: does nothing",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = true;
               controls.reset_ = false;

               auto constexpr original_output = -99342.2F;
               controls.output_ = original_output;

               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(controls.output_, is_equal_to(original_output));
             }));

    add("with run high: with gate low: does nothing",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = true;
               controls.gate_ = false;

               auto constexpr original_output = -992.223F;
               controls.output_ = original_output;

               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(controls.output_, is_equal_to(original_output));
             }));

    add("with run high: if gate rises: executes first step with gate edge "
        "cleared",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = true;
               controls.gate_ = true;

               auto constexpr first_enabled_step = 3;
               step_selector.first_ = first_enabled_step;

               step_controller.event_ = StepEvent::Generated;

               auto constexpr sample_time = 0.39947F;
               sequence_controller.execute(sample_time);

               t.assert_that(step_controller.entered_step_,
                             is_equal_to(first_enabled_step));
               t.assert_that(step_controller.executed_latch_,
                             is_equal_to(high_latch));
               t.assert_that(step_controller.executed_sample_time_,
                             is_equal_to(sample_time));
             }));

    add("with run high: if gate rises: does nothing if no first step",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = true;
               controls.gate_ = true;

               step_selector.first_ = -1;

               step_controller.event_ = StepEvent::Generated;

               auto constexpr original_output = -2340.223F;
               controls.output_ = original_output;

               sequence_controller.execute(0.F);

               t.assert_that(step_controller.called_, is_false);
               t.assert_that(controls.output_, is_equal_to(original_output));
             }));

    add("with run low: with reset low: does nothing",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = false;
               controls.reset_ = false;

               auto constexpr original_output = 349.319F;
               controls.output_ = original_output;
               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(controls.output_, is_equal_to(original_output));
             }));

    add("with run low: if reset rises: does nothing",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = false;
               controls.reset_ = true;

               auto constexpr original_output = 349.319F;
               controls.output_ = original_output;
               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(controls.output_, is_equal_to(original_output));
             }));

    add("with run low: with gate low: does nothing",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.running_ = false;
               controls.gate_ = false;

               auto constexpr original_output = 349.319F;
               controls.output_ = original_output;
               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(controls.output_, is_equal_to(original_output));
             }));

    add("with run low: if gate rises: does nothing",
        test(when_idle,
             [](Tester &t, Controls &controls, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               controls.gate_ = true;

               auto constexpr original_output = 349.319F;
               controls.output_ = original_output;
               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(controls.output_, is_equal_to(original_output));
             }));
  }
};
__attribute__((unused)) static auto _ = SequenceControllerIdleSuite{};
} // namespace curve_sequencer
} // namespace test