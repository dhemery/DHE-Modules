#include "./fixtures/controller-fixture.h"
#include "dheunit/assertions.h"

namespace test {
namespace scannibal {
using dhe::unit::is_equal_to;
using dhe::unit::is_near;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class SequenceControllerSuite : Suite {
public:
  SequenceControllerSuite() : Suite{"dhe::scannibal::Controller"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("phase voltage 0 executes step 0 at phase 0",
        test([](Tester &t, Module &module, Generator &generator,
                Controller &controller) {
          auto constexpr phase_in = 0.F;
          auto constexpr length = 5;

          module.phase_ = phase_in;
          module.length_ = length;

          // Give all steps the same weight
          for (auto i = 0; i < length; i++) {
            module.duration_[i] = 1.F;
          }

          auto constexpr expected_step = 0;
          auto constexpr expected_phase = 0.F;

          controller.execute();

          t.assert_that("module step", module.step_number_,
                        is_equal_to(expected_step));
          t.assert_that("module phase", module.step_phase_,
                        is_equal_to(expected_phase));
          t.assert_that("generator step", generator.step_,
                        is_near(expected_phase, 1e-4F));
          t.assert_that("generator phase", generator.phase_,
                        is_near(expected_phase, 1e-4F));
        }));

    add("phase voltage 10 executes selection end step at phase 1",
        test([](Tester &t, Module &module, Generator &generator,
                Controller &controller) {
          auto constexpr phase_in = 10.F;
          auto constexpr length = 5;

          module.phase_ = phase_in;
          module.length_ = length;

          auto constexpr expected_step = length - 1;
          auto constexpr expected_phase = 1.F;

          // Give all steps the same weight
          for (auto i = 0; i < length; i++) {
            module.duration_[i] = 1.F;
          }

          controller.execute();

          t.assert_that("module step", module.step_number_,
                        is_equal_to(expected_step));
          t.assert_that("module phase", module.step_phase_,
                        is_near(expected_phase, 1e-4F));
          t.assert_that("generator step", generator.step_,
                        is_equal_to(expected_step));
          t.assert_that("generator phase", generator.phase_,
                        is_near(expected_phase, 1e-4F));
        }));

    add("if equal durations: "
        "other phase voltage executes interpolated step and phase",
        test([](Tester &t, Module &module, Generator &generator,
                Controller &controller) {
          // Execute middle position of sequence
          auto constexpr phase_in = 5.F;

          auto constexpr length = 5;

          // Give all steps the same weight
          for (auto i = 0; i < length; i++) {
            module.duration_[i] = 1.F;
          }

          // Odd length, so middle position will be halfway through middle step
          auto constexpr middle_step = length / 2;
          auto constexpr middle_step_phase = 0.5F;

          module.phase_ = phase_in;
          module.length_ = length;

          controller.execute();

          t.assert_that("module step", module.step_number_,
                        is_equal_to(middle_step));
          t.assert_that("module phase", module.step_phase_,
                        is_near(middle_step_phase, 1e-4F));
          t.assert_that("generator step", generator.step_,
                        is_equal_to(middle_step));
          t.assert_that("generator phase", generator.phase_,
                        is_near(middle_step_phase, 1e-4F));
        }));
  }
};

__attribute__((unused)) static auto start_suite = SequenceControllerSuite{};
} // namespace scannibal
} // namespace test
