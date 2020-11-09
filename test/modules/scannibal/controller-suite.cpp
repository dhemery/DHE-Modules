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
    add("maps phase 0V to step 0 phase 0",
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
                        is_equal_to(expected_phase));
          t.assert_that("generator phase", generator.phase_,
                        is_equal_to(expected_phase));
        }));

    add("maps phase 10V to end step phase 1",
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
                        is_equal_to(expected_phase));
          t.assert_that("generator step", generator.step_,
                        is_equal_to(expected_step));
          t.assert_that("generator phase", generator.phase_,
                        is_equal_to(expected_phase));
        }));

    add("if equal durations: "
        "maps input phase (0V, 10V) to interpolated step and phase",
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

    add("wraps phase < 0V into range [0V, 10V)",
        test([](Tester &t, Module &module, Generator &generator,
                Controller &controller) {
          auto constexpr in_range_phase = 0.F;
          auto constexpr out_of_range_phase = in_range_phase - 20.F;
          auto constexpr length = 5;

          module.length_ = length;

          // Give all steps the same weight
          for (auto i = 0; i < length; i++) {
            module.duration_[i] = 1.F;
          }

          // Execute with in-bounds phase to get expected results
          module.phase_ = in_range_phase;
          controller.execute();
          auto const expected_module_step = module.step_number_;
          auto const expected_module_phase = module.step_phase_;
          auto const expected_generator_step = generator.step_;
          auto const expected_generator_phase = generator.phase_;

          module.phase_ = out_of_range_phase;
          controller.execute();

          t.assert_that("module step", module.step_number_,
                        is_equal_to(expected_module_step));
          t.assert_that("module phase", module.step_phase_,
                        is_near(expected_module_phase, 1e-4F));
          t.assert_that("generator step", generator.step_,
                        is_equal_to(expected_generator_step));
          t.assert_that("generator phase", generator.phase_,
                        is_near(expected_generator_phase, 1e-4F));
        }));

    add("wraps phase > 10V into range (0V, 10V]",
        test([](Tester &t, Module &module, Generator &generator,
                Controller &controller) {
          auto constexpr in_range_phase = 10.F;
          auto constexpr out_of_range_phase = in_range_phase + 30.F;
          auto constexpr length = 5;

          module.length_ = length;

          // Give all steps the same weight
          for (auto i = 0; i < length; i++) {
            module.duration_[i] = 1.F;
          }

          // Execute with in-bounds phase to get expected results
          module.phase_ = in_range_phase;
          controller.execute();
          auto const expected_module_step = module.step_number_;
          auto const expected_module_phase = module.step_phase_;
          auto const expected_generator_step = generator.step_;
          auto const expected_generator_phase = generator.phase_;

          module.phase_ = out_of_range_phase;
          controller.execute();

          t.assert_that("module step", module.step_number_,
                        is_equal_to(expected_module_step));
          t.assert_that("module phase", module.step_phase_,
                        is_near(expected_module_phase, 1e-4F));
          t.assert_that("generator step", generator.step_,
                        is_equal_to(expected_generator_step));
          t.assert_that("generator phase", generator.phase_,
                        is_near(expected_generator_phase, 1e-4F));
        }));
  }
};

__attribute__((unused)) static auto start_suite = SequenceControllerSuite{};
} // namespace scannibal
} // namespace test
