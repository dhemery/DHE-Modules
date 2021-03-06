#include "./fixtures/sequence-controller-fixture.h"
#include "dheunit/assertions.h"
#include <helpers/latches.h>

namespace test {
namespace sequencizer {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

static inline void when_idle(Module &module, StepSelector & /*step_selector*/,
                             StepController & /*step_controller*/,
                             SequenceController &sequence_controller) {
  module.running_ = false;
  module.reset_ = false;
  sequence_controller.execute(0.F);
}

class SequenceControllerIdleSuite : Suite {
public:
  SequenceControllerIdleSuite()
      : Suite{"dhe::sequencizer::SequenceController: idle"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("with run high: with reset low: does nothing",
        test(when_idle,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = true;
               module.reset_ = false;

               auto constexpr original_output = -99342.2F;
               module.output_ = original_output;

               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(module.output_, is_equal_to(original_output));
             }));

    add("with run high: with gate low: does nothing",
        test(when_idle,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = true;
               module.gate_ = false;

               auto constexpr original_output = -992.223F;
               module.output_ = original_output;

               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(module.output_, is_equal_to(original_output));
             }));

    add("with run high: "
        "if gate rises: executes first step with gate edge cleared",
        test(when_idle,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = true;
               module.gate_ = true;

               auto constexpr first_enabled_step = 3;
               step_selector.first_ = first_enabled_step;

               step_controller.status_ = StepStatus::Generating;

               auto constexpr sample_time = 0.39947F;
               sequence_controller.execute(sample_time);

               t.assert_that(step_controller.entered_step_,
                             is_equal_to(first_enabled_step));
               t.assert_that(step_controller.executed_latch_,
                             is_equal_to(high_latch));
               t.assert_that(step_controller.executed_sample_time_,
                             is_equal_to(sample_time));
             }));

    add("with run high: "
        "if gate rises: does nothing if no first step",
        test(when_idle,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = true;
               module.gate_ = true;

               step_selector.first_ = -1;

               auto constexpr original_output = -2340.223F;
               module.output_ = original_output;

               sequence_controller.execute(0.F);

               t.assert_that(step_controller.called_, is_false);
               t.assert_that(module.output_, is_equal_to(original_output));
             }));

    add("with run low: with reset low: does nothing",
        test(when_idle,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = false;
               module.reset_ = false;

               auto constexpr original_output = 349.319F;
               module.output_ = original_output;
               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(module.output_, is_equal_to(original_output));
             }));

    add("with run low: "
        "if reset rises: "
        "does nothing",
        test(when_idle,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = false;
               module.reset_ = true;

               auto constexpr original_output = 349.319F;
               module.output_ = original_output;
               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(module.output_, is_equal_to(original_output));
             }));

    add("with run low: with gate low: does nothing",
        test(when_idle,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.running_ = false;
               module.gate_ = false;

               auto constexpr original_output = 349.319F;
               module.output_ = original_output;
               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(module.output_, is_equal_to(original_output));
             }));

    add("with run low: if gate rises: does nothing",
        test(when_idle,
             [](Tester &t, Module &module, StepSelector &step_selector,
                StepController &step_controller,
                SequenceController &sequence_controller) {
               module.gate_ = true;

               auto constexpr original_output = 349.319F;
               module.output_ = original_output;
               sequence_controller.execute(0.1F);

               t.assert_that(step_selector.called_, is_false);
               t.assert_that(step_controller.called_, is_false);
               t.assert_that(module.output_, is_equal_to(original_output));
             }));
  }
};
__attribute__((unused)) static auto _ = SequenceControllerIdleSuite{};
} // namespace sequencizer
} // namespace test
