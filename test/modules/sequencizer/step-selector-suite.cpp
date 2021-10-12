#include "./fixtures/step-selector-fixture.h"

#include "helpers/assertions.h"

#include "dheunit/test.h"

namespace test {
namespace sequencizer {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;
using test::is_less_than;

static inline void enable_all(Signals &signals) {
  for (auto &enabled : signals.enabled_) {
    enabled = true;
  }
}

class StepSelectorFirstSuite : public Suite {
public:
  StepSelectorFirstSuite() : Suite{"dhe::sequencizer::StepSelector: first()"} {}
  void run(Tester &t) override {
    t.run("is selection start if enabled",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            auto constexpr selection_start = 3;
            signals.start_ = selection_start;
            signals.length_ = 4;
            signals.enabled_[selection_start] = true;

            auto const first = selector.first();

            assert_that(t, first, is_equal_to(selection_start));
          }));

    t.run("is successor of selection start if selection start is disabled",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            auto constexpr start = 3;
            signals.start_ = start;
            signals.length_ = 4;

            auto constexpr enabled_selected_step = 5;
            signals.enabled_[enabled_selected_step] = true;

            auto const first = selector.first();
            auto const successor = selector.successor(start);

            assert_that(t, first, is_equal_to(successor));
          }));

    t.run("is no step if no selected step is enabled",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // All selected ...
            signals.start_ = 0;
            signals.length_ = step_count;

            // ... but none enabled
            for (auto &enabled : signals.enabled_) {
              enabled = false;
            }

            auto const first = selector.first();

            assert_that(t, first, is_less_than(0));
          }));
  }
};

class StepSelectorSuccessorSuite : public Suite {
public:
  StepSelectorSuccessorSuite()
      : Suite{"dhe::sequencizer::StepSelector: successor(s)"} {}
  void run(Tester &t) override {
    t.run("can be above s",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // [0 1 2 3 4 5 6 7]
            signals.start_ = 0;
            signals.length_ = step_count;

            auto constexpr given_step = 2;

            // Above given step in selection
            auto constexpr expected_successor = given_step + 2;

            signals.enabled_[expected_successor] = true;

            auto const successor = selector.successor(given_step);

            assert_that(t, successor, is_equal_to(expected_successor));
          }));

    t.run("can be below s if selection wraps",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // 0 1] 2 3 4 5 [6 7
            auto constexpr start = 6;
            signals.start_ = start;
            signals.length_ = 4;

            auto constexpr expected_successor = 1; // Must wrap to find
            signals.enabled_[expected_successor] = true;

            auto const successor = selector.successor(start);

            assert_that(t, successor, is_equal_to(expected_successor));
          }));

    t.run("can be selection end",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // 0 1 [2 3 4] 5 6 7
            auto constexpr start = 2;
            auto constexpr length = 3;
            signals.start_ = start;
            signals.length_ = length;

            auto constexpr end = start + length - 1;
            signals.enabled_[end] = true;

            auto const successor = selector.successor(start);

            assert_that(t, successor, is_equal_to(end));
          }));

    t.run("if no selected step is enabled: is no step",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // 0 [1 2 3 4] 5 6 7
            auto constexpr start = 1;
            auto constexpr length = 4;
            signals.start_ = start;
            signals.length_ = length;

            // Enabled but not in selection
            auto const only_enabled_step = 6;
            signals.enabled_[only_enabled_step] = true;

            auto const successor = selector.successor(start);

            assert_that(t, successor, is_less_than(0));
          }));

    t.run("if selection wraps and no selected step is enabled: is no step",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // 0 1 2 3] 4 [5 6 7
            auto constexpr start = 5;
            auto constexpr length = 7;
            signals.start_ = start;
            signals.length_ = length;

            // Enabled but not in selection
            auto const only_enabled_step = 4;
            signals.enabled_[only_enabled_step] = true;

            auto const successor = selector.successor(start);

            assert_that(t, successor, is_less_than(0));
          }));

    t.run("if s is below selection: is no step",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // 0 1 2 3 [4 5 6] 7
            auto constexpr start = 4;
            auto constexpr length = 3;
            signals.start_ = start;
            signals.length_ = length;
            enable_all(signals);

            // Lower than any selected step
            auto constexpr given_step = 3;

            auto const successor = selector.successor(given_step);

            assert_that(t, successor, is_less_than(0));
          }));

    t.run("if s is above selection: is no step",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // 0 1 2 3 [4 5 6] 7
            auto constexpr start = 4;
            auto constexpr length = 3;
            signals.start_ = start;
            signals.length_ = length;
            enable_all(signals);

            // Higher than any selected step
            auto constexpr given_step = 7;

            auto const successor = selector.successor(given_step);

            assert_that(t, successor, is_less_than(0));
          }));

    t.run("if s is not in wrapped selection: is no step",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            // 0 1] 2 3 4 5 [6 7
            auto constexpr start = 6;
            auto constexpr length = 4;
            signals.start_ = start;
            signals.length_ = length;
            enable_all(signals);

            // Above selection end, below selection start
            auto constexpr given_step = 5;

            auto const successor = selector.successor(given_step);

            assert_that(t, successor, is_less_than(0));
          }));

    t.run("if s is selection end: is no step",
          test([](Tester &t, Signals &signals, StepSelector &selector) {
            auto constexpr end = 3;
            signals.enabled_[end] = true;
            enable_all(signals);

            // 0 [1 2 3] 4 5 6 7
            auto constexpr start = 1;
            auto constexpr length = end;
            signals.start_ = start;
            signals.length_ = length;

            auto const successor = selector.successor(end);

            assert_that(t, successor, is_less_than(0));
          }));
  }
};

__attribute__((unused)) static auto first_suite = StepSelectorFirstSuite{};
__attribute__((unused)) static auto successor_suite =
    StepSelectorSuccessorSuite{};
} // namespace sequencizer
} // namespace test
