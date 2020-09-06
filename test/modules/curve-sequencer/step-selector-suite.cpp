#include "./fixtures/step-selector-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::is_less_than;
using dhe::unit::Suite;
using dhe::unit::Tester;

class StepSelectorFirstSuite : public Suite {
public:
  StepSelectorFirstSuite()
      : Suite{"dhe::curve_sequencer::StepSelector: first()"} {}
  void register_tests(dhe::unit::TestRegistrar add) override {
    add("is selection start if enabled",
        test([](Tester &t, Module &module, StepSelector &selector) {
          auto constexpr selection_start = 3;
          module.start_ = selection_start;
          module.length_ = 4;
          module.enabled_[selection_start] = true;

          auto const first = selector.first();

          t.assert_that(first, is_equal_to(selection_start));
        }));

    add("is successor of selection start if selection start is disabled",
        test([](Tester &t, Module &module, StepSelector &selector) {
          auto constexpr start = 3;
          module.start_ = start;
          module.length_ = 4;

          auto constexpr enabled_selected_step = 5;
          module.enabled_[enabled_selected_step] = true;

          auto const first = selector.first();
          auto const successor = selector.successor(start, false);

          t.assert_that(first, is_equal_to(successor));
        }));

    add("is successor earlier in selection if sequencer is looping",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // [0 1 2 3 4 5 6 7]
          module.start_ = 0;
          module.length_ = step_count;

          auto constexpr step = 3;

          // Earlier in selection
          module.enabled_[step - 2] = true;

          auto const successor = selector.successor(step, true);
          auto const first = selector.first();

          t.assert_that(first, is_equal_to(successor));
        }));

    add("is no step if no selected step is enabled",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // All selected ...
          module.start_ = 0;
          module.length_ = step_count;

          // ... but none enabled
          for (auto &enabled : module.enabled_) {
            enabled = false;
          }

          auto const first = selector.first();

          t.assert_that(first, is_less_than(0));
        }));
  }
};

class StepSelectorSuccessorSuite : public Suite {
public:
  StepSelectorSuccessorSuite()
      : Suite{"dhe::curve_sequencer::StepSelector: successor(s)"} {}
  void register_tests(dhe::unit::TestRegistrar add) override {
    add("can be above s",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // [0 1 2 3 4 5 6 7]
          module.start_ = 0;
          module.length_ = step_count;

          auto constexpr given_step = 2;

          // Above given step in selection
          auto constexpr expected_successor = given_step + 2;

          module.enabled_[expected_successor] = true;

          auto const successor = selector.successor(given_step, false);

          t.assert_that(successor, is_equal_to(expected_successor));
        }));

    add("can be below s if selection wraps",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // 0 1] 2 3 4 5 [6 7
          auto constexpr start = 6;
          module.start_ = start;
          module.length_ = 4;

          auto constexpr expected_successor = 1; // Must wrap to find
          module.enabled_[expected_successor] = true;

          auto const successor = selector.successor(start, false);

          t.assert_that(successor, is_equal_to(expected_successor));
        }));

    add("can be selection end",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // 0 1 [2 3 4] 5 6 7
          auto constexpr start = 2;
          auto constexpr length = 3;
          module.start_ = start;
          module.length_ = length;

          auto constexpr end = start + length - 1;
          module.enabled_[end] = true;

          auto const successor = selector.successor(start, false);

          t.assert_that(successor, is_equal_to(end));
        }));

    add("can be s if sequencer is looping",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // [0 1 2 3 4 5 6 7]
          module.start_ = 0;
          module.length_ = step_count;
          auto constexpr step = 3;
          module.enabled_[step] = true;

          auto const successor = selector.successor(step, true);

          t.assert_that(successor, is_equal_to(step));
        }));

    add("is no step: if no selected step is enabled",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // 0 [1 2 3 4] 5 6 7
          auto constexpr start = 1;
          auto constexpr length = 4;
          module.start_ = start;
          module.length_ = length;

          // Enabled but not in selection
          auto const only_enabled_step = 6;
          module.enabled_[only_enabled_step] = true;

          auto const successor = selector.successor(start, true);

          t.assert_that(successor, is_less_than(0));
        }));

    add("is no step: if selection wraps and no selected step is enabled",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // 0 1 2 3] 4 [5 6 7
          auto constexpr start = 5;
          auto constexpr length = 7;
          module.start_ = start;
          module.length_ = length;

          // Enabled but not in selection
          auto const only_enabled_step = 4;
          module.enabled_[only_enabled_step] = true;

          auto const successor = selector.successor(start, true);

          t.assert_that(successor, is_less_than(0));
        }));

    add("if s is below selection",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // 0 1 2 3 [4 5 6] 7
          auto constexpr start = 4;
          auto constexpr length = 3;
          module.start_ = start;
          module.length_ = length;

          // Lower than any selected step
          auto constexpr given_step = 3;
          module.enabled_[given_step] = true;

          auto const successor = selector.successor(given_step, true);

          t.assert_that(successor, is_less_than(0));
        }));

    add("if s is above selection",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // 0 1 2 3 [4 5 6] 7
          auto constexpr start = 4;
          auto constexpr length = 3;
          module.start_ = start;
          module.length_ = length;

          // Higher than any selected step
          auto constexpr given_step = 7;
          module.enabled_[given_step] = true;

          auto const successor = selector.successor(given_step, true);

          t.assert_that(successor, is_less_than(0));
        }));

    add("if s is not in wrapped selection",
        test([](Tester &t, Module &module, StepSelector &selector) {
          // 0 1] 2 3 4 5 [6 7
          auto constexpr start = 6;
          auto constexpr length = 4;
          module.start_ = start;
          module.length_ = length;

          // Above selection end, below selection start
          auto constexpr given_step = 5;
          module.enabled_[given_step] = true;

          auto const successor = selector.successor(given_step, true);

          t.assert_that(successor, is_less_than(0));
        }));

    add("if s is selection end and sequencer is not looping",
        test([](Tester &t, Module &module, StepSelector &selector) {
          auto constexpr end = 3;
          module.enabled_[end] = true;

          // 0 [1 2 3] 4 5 6 7
          auto constexpr start = 1;
          auto constexpr length = end;
          module.start_ = start;
          module.length_ = length;

          auto const successor = selector.successor(end, false);

          t.assert_that(successor, is_less_than(0));
        }));
  }
};

__attribute__((unused)) static auto first_suite = StepSelectorFirstSuite{};
__attribute__((unused)) static auto successor_suite =
    StepSelectorSuccessorSuite{};
} // namespace curve_sequencer
} // namespace test
