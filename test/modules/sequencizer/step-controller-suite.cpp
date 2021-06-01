#include "fixtures/status-enums.h"
#include "fixtures/step-controller-fixture.h"
#include "helpers/assertions.h"
#include <dheunit/test.h>

namespace test {
namespace sequencizer {

using dhe::Latch;
using dhe::sequencizer::StepStatus;
using dhe::unit::Suite;
using test::is_equal_to;

class StepControllerSuite : public Suite {
public:
  StepControllerSuite() : Suite{"dhe::sequencizer::StepController"} {}
  void run(Tester &t) override {
    t.run("enter(s) starts the generator",
          test([](Tester &t, Interrupter & /**/, Generator &generator,
                  Sustainer /**/, StepController &step_controller) {
            auto constexpr step = 4;

            step_controller.enter(step);

            assert_that(t, generator.started_step_, is_equal_to(step));
          }));

    t.run("exit() stops the generator",
          test([](Tester &t, Interrupter & /**/, Generator &generator,
                  Sustainer & /**/, StepController &step_controller) {
            step_controller.exit();

            assert_that(t, "generator.stopped", generator.stopped_, is_true);
          }));

    t.run("execute() completes without generating if interrupted",
          test([](Tester &t, Interrupter &interrupter, Generator &generator,
                  Sustainer & /**/, StepController &step_controller) {
            auto constexpr step = 5;
            step_controller.enter(step);
            interrupter.is_interrupted_[step] = true;

            auto const status = step_controller.execute(Latch{}, 0.F);

            assert_that(t, "status", status, is_equal_to(StepStatus::Idle));
            assert_that(t, "generator.stopped", generator.stopped_, is_true);
          }));

    t.run("execute() generates if not interrupted",
          test([](Tester &t, Interrupter &interrupter, Generator &generator,
                  Sustainer & /**/, StepController &step_controller) {
            auto constexpr step = 6;
            step_controller.enter(step);

            interrupter.is_interrupted_[step] = false;
            auto constexpr sample_time = 0.117F;

            auto const status = step_controller.execute(Latch{}, sample_time);

            assert_that(t, "status", status,
                        is_equal_to(StepStatus::Generating));
            assert_that(t, "generator.sample_time", generator.sample_time_,
                        is_equal_to(sample_time));
            assert_that(t, "generator.stopped", generator.stopped_, is_false);
          }));

    t.run("execute() completes if not sustaining when generator completes",
          test([](Tester &t, Interrupter & /**/, Generator &generator,
                  Sustainer &sustainer, StepController &step_controller) {
            auto constexpr step = 7;
            step_controller.enter(step);
            generator.status_[step] = GeneratorStatus::Completed;
            sustainer.is_done_[step] = true;

            auto const status = step_controller.execute(Latch{}, 0.F);

            assert_that(t, "status", status, is_equal_to(StepStatus::Idle));
            assert_that(t, "generator.stopped", generator.stopped_, is_true);
          }));

    t.run(
        "execute() remains in progress if sustaining when generator completes",
        test([](Tester &t, Interrupter & /**/, Generator &generator,
                Sustainer &sustainer, StepController &step_controller) {
          auto constexpr step = 0;
          step_controller.enter(step);
          generator.status_[step] = GeneratorStatus::Completed;
          sustainer.is_done_[step] = false;

          auto const status = step_controller.execute(Latch{}, 0.F);

          assert_that(t, "status", status, is_equal_to(StepStatus::Sustaining));
          assert_that(t, "generator.stopped", generator.stopped_, is_false);
        }));

    t.run("execute() remains in progress if generator generates",
          test([](Tester &t, Interrupter & /**/, Generator &generator,
                  Sustainer & /**/, StepController &step_controller) {
            auto constexpr step = 0;
            step_controller.enter(step);
            generator.status_[step] = GeneratorStatus::Generating;

            auto const status = step_controller.execute(Latch{}, 0.F);

            assert_that(t, "status", status,
                        is_equal_to(StepStatus::Generating));
            assert_that(t, "generator.stopped", generator.stopped_, is_false);
          }));
  }
};

static auto _ = StepControllerSuite{};
} // namespace sequencizer
} // namespace test
