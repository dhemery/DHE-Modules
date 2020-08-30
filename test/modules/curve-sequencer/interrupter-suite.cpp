#include "./fixtures/interrupter-fixture.h"
#include "helpers/latches.h"
#include <dheunit/assertions.h>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;

class InterrupterSuite : public Suite {
public:
  InterrupterSuite() : Suite{"dhe::curve_sequencer::Interrupter"} {}
  void register_tests(dhe::unit::TestRegistrar add) override {
    add("interrupt=true, mode=GateRises, is_interrupted(s) iff gate rises",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 0;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateRises;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_true);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_false);
          t.assert_that("high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, low_latch),
                        is_false);
        }));

    add("interrupt=true, mode=GateFalls, is_interrupted(s) iff gate falls",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 1;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateFalls;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_true);
          t.assert_that("high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, low_latch),
                        is_false);
        }));

    add("interrupt=true, mode=GateChanges, is_interrupted(s) iff gate changes",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 2;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateChanges;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_true);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_true);
          t.assert_that("high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, low_latch),
                        is_false);
        }));

    add("interrupt=true, mode=GateIsHigh, is_interrupted(s) iff gate is high",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 3;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateIsHigh;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_true);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_false);
          t.assert_that("high gate",
                        interrupter.is_interrupted(step, high_latch), is_true);
          t.assert_that("low gate", interrupter.is_interrupted(step, low_latch),
                        is_false);
        }));

    add("interrupt=true, mode=GateIsLow, is_interrupted(s) iff gate is low",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 4;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateIsLow;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_true);
          t.assert_that("high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, low_latch),
                        is_true);
        }));

    add("interrupt=false, is_interrupted(s) regardless of mode or gate",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 5;
          controls.interrupt_on_trigger_[step] = false;

          controls.trigger_mode_[step] = TriggerMode::GateRises;
          t.assert_that("mode=GateRises: rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_false);
          t.assert_that("mode=GateRises: falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_false);
          t.assert_that("mode=GateRises: high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("mode=GateRises: low gate",
                        interrupter.is_interrupted(step, low_latch), is_false);

          controls.trigger_mode_[step] = TriggerMode::GateFalls;
          t.assert_that("mode=GateFalls: rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_false);
          t.assert_that("mode=GateFalls: falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_false);
          t.assert_that("mode=GateFalls: high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("mode=GateFalls: low gate",
                        interrupter.is_interrupted(step, low_latch), is_false);

          controls.trigger_mode_[step] = TriggerMode::GateChanges;
          t.assert_that("mode=GateChanges: rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_false);
          t.assert_that("mode=GateChanges: falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_false);
          t.assert_that("mode=GateChanges: high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("mode=GateChanges: low gate",
                        interrupter.is_interrupted(step, low_latch), is_false);

          controls.trigger_mode_[step] = TriggerMode::GateIsHigh;
          t.assert_that("mode=GateIsHigh: rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_false);
          t.assert_that("mode=GateIsHigh: falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_false);
          t.assert_that("mode=GateIsHigh: high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("mode=GateIsHigh: low gate",
                        interrupter.is_interrupted(step, low_latch), is_false);

          controls.trigger_mode_[step] = TriggerMode::GateIsLow;
          t.assert_that("mode=GateIsLow: rising gate",
                        interrupter.is_interrupted(step, rising_latch),
                        is_false);
          t.assert_that("mode=GateIsLow: falling gate",
                        interrupter.is_interrupted(step, falling_latch),
                        is_false);
          t.assert_that("mode=GateIsLow: high gate",
                        interrupter.is_interrupted(step, high_latch), is_false);
          t.assert_that("mode=GateIsLow: low gate",
                        interrupter.is_interrupted(step, low_latch), is_false);
        }));
  }
};

__attribute__((unused)) static auto _ = InterrupterSuite{};
} // namespace curve_sequencer
} // namespace test
