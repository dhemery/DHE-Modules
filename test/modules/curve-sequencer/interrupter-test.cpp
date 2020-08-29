#include "trigger-test.h"
#include <dheunit/assertions.h>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;

class InterrupterTest : public Suite {
public:
  InterrupterTest()
      : Suite{"dhe::curve_sequencer::Interrupter: is_interrupted(s)"} {}
  void register_tests(dhe::unit::TestRegistrar add) override {
    add("interrupt=true, mode=GateRises",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 0;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateRises;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_true);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate),
                        is_false);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);
        }));

    add("interrupt=true, mode=GateFalls",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 1;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateFalls;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate), is_true);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);
        }));

    add("interrupt=true, mode=GateChanges",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 2;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateChanges;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_true);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate), is_true);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);
        }));

    add("interrupt=true, mode=GateIsHigh",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 3;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateIsHigh;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_true);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate),
                        is_false);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_true);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);
        }));

    add("interrupt=true, mode=GateIsLow",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 4;
          controls.interrupt_on_trigger_[step] = true;
          controls.trigger_mode_[step] = TriggerMode::GateIsLow;

          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate), is_true);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_true);
        }));

    add("interrupt=false",
        test([](Tester &t, Controls &controls, Interrupter &interrupter) {
          auto constexpr step = 5;
          controls.interrupt_on_trigger_[step] = false;

          controls.trigger_mode_[step] = TriggerMode::GateRises;
          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate),
                        is_false);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);

          controls.trigger_mode_[step] = TriggerMode::GateFalls;
          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate),
                        is_false);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);

          controls.trigger_mode_[step] = TriggerMode::GateChanges;
          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate),
                        is_false);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);

          controls.trigger_mode_[step] = TriggerMode::GateIsHigh;
          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate),
                        is_false);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);

          controls.trigger_mode_[step] = TriggerMode::GateIsLow;
          t.assert_that("rising gate",
                        interrupter.is_interrupted(step, risingGate), is_false);
          t.assert_that("falling gate",
                        interrupter.is_interrupted(step, fallingGate),
                        is_false);
          t.assert_that("high gate", interrupter.is_interrupted(step, highGate),
                        is_false);
          t.assert_that("low gate", interrupter.is_interrupted(step, lowGate),
                        is_false);
        }));
  }
};

__attribute__((unused)) static auto _ = InterrupterTest{};
} // namespace curve_sequencer
} // namespace test
