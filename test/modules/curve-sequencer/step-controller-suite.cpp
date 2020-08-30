#include "./fixtures/step-controller-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

using dhe::Latch;
using dhe::curve_sequencer::StepEvent;
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::TestRegistrar;

class StepControllerSuite : public Suite {
public:
  StepControllerSuite() : Suite{"dhe::curve_sequencer::StepController"} {}
  void register_tests(TestRegistrar add) override {
    add("enter(s) starts the generator",
        test([](Tester &t, Interrupter & /**/, Generator &generator,
                Sustainer /**/, StepController &step_controller) {
          auto constexpr step = 4;

          step_controller.enter(step);

          t.assert_that(generator.started_step_, is_equal_to(step));
        }));

    add("exit() stops the generator",
        test([](Tester &t, Interrupter & /**/, Generator &generator,
                Sustainer & /**/, StepController &step_controller) {
          step_controller.exit();

          t.assert_that("generator.stopped", generator.stopped_, is_true);
        }));

    add("execute() completes without generating if interrupted",
        test([](Tester &t, Interrupter &interrupter, Generator &generator,
                Sustainer & /**/, StepController &step_controller) {
          auto constexpr step = 5;
          step_controller.enter(step);
          interrupter.is_interrupted_[step] = true;

          auto const result = step_controller.execute(Latch{}, 0.F);

          t.assert_that("result", result, is_equal_to(StepEvent::Completed));
          t.assert_that("generator.stopped", generator.stopped_, is_true);
        }));

    add("execute() generates if not interrupted",
        test([](Tester &t, Interrupter &interrupter, Generator &generator,
                Sustainer & /**/, StepController &step_controller) {
          auto constexpr step = 6;
          step_controller.enter(step);

          interrupter.is_interrupted_[step] = false;
          auto constexpr sample_time = 0.117F;

          auto const result = step_controller.execute(Latch{}, sample_time);

          t.assert_that("result", result, is_equal_to(StepEvent::Generated));
          t.assert_that("generator.sample_time", generator.sample_time_,
                        is_equal_to(sample_time));
          t.assert_that("generator.stopped", generator.stopped_, is_false);
        }));

    add("execute() stops generating if not sustaining when curve is done",
        test([](Tester &t, Interrupter & /**/, Generator &generator,
                Sustainer &sustainer, StepController &step_controller) {
          auto constexpr step = 7;
          step_controller.enter(step);
          generator.curve_is_done_[step] = true;
          sustainer.is_done_[step] = true;

          auto const result = step_controller.execute(Latch{}, 0.F);

          t.assert_that("result", result, is_equal_to(StepEvent::Completed));
          t.assert_that("generator.stopped", generator.stopped_, is_true);
        }));

    add("execute() remains generating if sustaining when curve is done",
        test([](Tester &t, Interrupter & /**/, Generator &generator,
                Sustainer &sustainer, StepController &step_controller) {
          auto constexpr step = 0;
          step_controller.enter(step);
          generator.curve_is_done_[step] = true;
          sustainer.is_done_[step] = false;

          auto const result = step_controller.execute(Latch{}, 0.F);

          t.assert_that("result", result, is_equal_to(StepEvent::Generated));
          t.assert_that("generator.stopped", generator.stopped_, is_false);
        }));

    add("execute() remains generating if curve is not done",
        test([](Tester &t, Interrupter & /**/, Generator &generator,
                Sustainer & /**/, StepController &step_controller) {
          auto constexpr step = 0;
          step_controller.enter(step);
          generator.curve_is_done_[step] = false;

          auto const result = step_controller.execute(Latch{}, 0.F);

          t.assert_that("result", result, is_equal_to(StepEvent::Generated));
          t.assert_that("generator.stopped", generator.stopped_, is_false);
        }));
  }
};

__attribute__((unused)) static auto _ = StepControllerSuite{};
} // namespace curve_sequencer
} // namespace test
