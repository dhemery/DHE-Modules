#include "./fixtures/sustainer-fixture.h"
#include "helpers/latches.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;

class SustainerSuite : public Suite {
public:
  SustainerSuite() : Suite{"dhe::curve_sequencer::Sustainer"} {}
  void register_tests(dhe::unit::TestRegistrar add) override {
    add("CompletionMode::Sustain, mode=GateRises, is_done(s) iff gate rises",
        test([](Tester &t, Controls &controls, Sustainer &interrupter) {
          auto constexpr step = 0;
          controls.completion_mode_[step] = CompletionMode::Sustain;
          controls.trigger_mode_[step] = TriggerMode::GateRises;

          t.assert_that("rising gate", interrupter.is_done(step, rising_latch),
                        is_true);
          t.assert_that("falling gate",
                        interrupter.is_done(step, falling_latch), is_false);
          t.assert_that("high gate", interrupter.is_done(step, high_latch),
                        is_false);
          t.assert_that("low gate", interrupter.is_done(step, low_latch),
                        is_false);
        }));

    add("CompletionMode::Sustain, mode=GateFalls, is_done(s) iff gate falls",
        test([](Tester &t, Controls &controls, Sustainer &sustainer) {
          auto constexpr step = 1;
          controls.completion_mode_[step] = CompletionMode::Sustain;
          controls.trigger_mode_[step] = TriggerMode::GateFalls;

          t.assert_that("rising gate", sustainer.is_done(step, rising_latch),
                        is_false);
          t.assert_that("falling gate", sustainer.is_done(step, falling_latch),
                        is_true);
          t.assert_that("high gate", sustainer.is_done(step, high_latch),
                        is_false);
          t.assert_that("low gate", sustainer.is_done(step, low_latch),
                        is_false);
        }));

    add("CompletionMode::Sustain, mode=GateChanges, is_done(s) iff gate "
        "changes",
        test([](Tester &t, Controls &controls, Sustainer &sustainer) {
          auto constexpr step = 2;
          controls.completion_mode_[step] = CompletionMode::Sustain;
          controls.trigger_mode_[step] = TriggerMode::GateChanges;

          t.assert_that("rising gate", sustainer.is_done(step, rising_latch),
                        is_true);
          t.assert_that("falling gate", sustainer.is_done(step, falling_latch),
                        is_true);
          t.assert_that("high gate", sustainer.is_done(step, high_latch),
                        is_false);
          t.assert_that("low gate", sustainer.is_done(step, low_latch),
                        is_false);
        }));

    add("CompletionMode::Sustain, mode=GateIsHigh, is_done(s) iff gate is high",
        test([](Tester &t, Controls &controls, Sustainer &sustainer) {
          auto constexpr step = 3;
          controls.completion_mode_[step] = CompletionMode::Sustain;
          controls.trigger_mode_[step] = TriggerMode::GateIsHigh;

          t.assert_that("rising gate", sustainer.is_done(step, rising_latch),
                        is_true);
          t.assert_that("falling gate", sustainer.is_done(step, falling_latch),
                        is_false);
          t.assert_that("high gate", sustainer.is_done(step, high_latch),
                        is_true);
          t.assert_that("low gate", sustainer.is_done(step, low_latch),
                        is_false);
        }));

    add("CompletionMode::Sustain, mode=GateIsLow, is_done(s) iff gate is low",
        test([](Tester &t, Controls &controls, Sustainer &sustainer) {
          auto constexpr step = 4;
          controls.completion_mode_[step] = CompletionMode::Sustain;
          controls.trigger_mode_[step] = TriggerMode::GateIsLow;

          t.assert_that("rising gate", sustainer.is_done(step, rising_latch),
                        is_false);
          t.assert_that("falling gate", sustainer.is_done(step, falling_latch),
                        is_true);
          t.assert_that("high gate", sustainer.is_done(step, high_latch),
                        is_false);
          t.assert_that("low gate", sustainer.is_done(step, low_latch),
                        is_true);
        }));

    add("CompletionMode::Advance, is_done(s) regardless of mode or gate",
        test([](Tester &t, Controls &controls, Sustainer &sustainer) {
          auto constexpr step = 5;
          controls.completion_mode_[step] = CompletionMode::Advance;

          controls.trigger_mode_[step] = TriggerMode::GateRises;
          t.assert_that("mode=GateRises: rising gate",
                        sustainer.is_done(step, rising_latch), is_true);
          t.assert_that("mode=GateRises: falling gate",
                        sustainer.is_done(step, falling_latch), is_true);
          t.assert_that("mode=GateRises: high gate",
                        sustainer.is_done(step, high_latch), is_true);
          t.assert_that("mode=GateRises: low gate",
                        sustainer.is_done(step, low_latch), is_true);

          controls.trigger_mode_[step] = TriggerMode::GateFalls;
          t.assert_that("mode=GateFalls: rising gate",
                        sustainer.is_done(step, rising_latch), is_true);
          t.assert_that("mode=GateFalls: falling gate",
                        sustainer.is_done(step, falling_latch), is_true);
          t.assert_that("mode=GateFalls: high gate",
                        sustainer.is_done(step, high_latch), is_true);
          t.assert_that("mode=GateFalls: low gate",
                        sustainer.is_done(step, low_latch), is_true);

          controls.trigger_mode_[step] = TriggerMode::GateChanges;
          t.assert_that("mode=GateChanges: rising gate",
                        sustainer.is_done(step, rising_latch), is_true);
          t.assert_that("mode=GateChanges: falling gate",
                        sustainer.is_done(step, falling_latch), is_true);
          t.assert_that("mode=GateChanges: high gate",
                        sustainer.is_done(step, high_latch), is_true);
          t.assert_that("mode=GateChanges: low gate",
                        sustainer.is_done(step, low_latch), is_true);

          controls.trigger_mode_[step] = TriggerMode::GateIsHigh;
          t.assert_that("mode=GateIsHigh: rising gate",
                        sustainer.is_done(step, rising_latch), is_true);
          t.assert_that("mode=GateIsHigh: falling gate",
                        sustainer.is_done(step, falling_latch), is_true);
          t.assert_that("mode=GateIsHigh: high gate",
                        sustainer.is_done(step, high_latch), is_true);
          t.assert_that("mode=GateIsHigh: low gate",
                        sustainer.is_done(step, low_latch), is_true);

          controls.trigger_mode_[step] = TriggerMode::GateIsLow;
          t.assert_that("mode=GateIsLow: rising gate",
                        sustainer.is_done(step, rising_latch), is_true);
          t.assert_that("mode=GateIsLow: falling gate",
                        sustainer.is_done(step, falling_latch), is_true);
          t.assert_that("mode=GateIsLow: high gate",
                        sustainer.is_done(step, high_latch), is_true);
          t.assert_that("mode=GateIsLow: low gate",
                        sustainer.is_done(step, low_latch), is_true);
        }));
  }
};

__attribute__((unused)) static auto _ = SustainerSuite{};
} // namespace curve_sequencer
} // namespace test
