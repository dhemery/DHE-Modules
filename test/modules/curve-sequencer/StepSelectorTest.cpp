#include "modules/curve-sequencer/StepSelector.h"

#include "doctest/doctest.h"
#include "fake/Controls.h"

namespace test {
namespace curve_sequencer {
  namespace step_selector {
  static auto constexpr stepCount{8};
  using dhe::curve_sequencer::StepSelector;


  static inline void givenEnabled(fake::Controls &controls, int enabledStep) {
    controls.isEnabled = [=](int step) -> bool { return step == enabledStep; };
  }
  static inline void givenSelection(fake::Controls &controls, int start, int length) {
    controls.selectionStart = [=]() -> int { return start; };
    controls.selectionLength = [=]() -> int { return length; };
  }

  TEST_CASE("curve_sequencer::StepSelector") {
    fake::Controls controls{};
    StepSelector<fake::Controls> selector{controls, stepCount};

    SUBCASE("successor()") {
      SUBCASE("can be") {
        SUBCASE("above given step") {
          givenSelection(controls, 0, stepCount); // [0 1 2 3 4 5 6 7]
          auto const givenStep = 2;
          auto const expectedSuccessor = givenStep + 2; // Later in selection
          givenEnabled(controls, expectedSuccessor);

          auto const successor = selector.successor(givenStep, false);

          CHECK_EQ(successor, expectedSuccessor);
        }

        SUBCASE("below given step if selection wraps") {
          auto const selectionStart = 6;
          auto const selectionLength = 4;
          givenSelection(controls, selectionStart, selectionLength); // 0 1] 2 3 4 5 [6 7
          auto const expectedSuccessor = 1;                          // Must wrap to find
          givenEnabled(controls, expectedSuccessor);

          auto const successor = selector.successor(selectionStart, false);

          CHECK_EQ(successor, expectedSuccessor);
        }

        SUBCASE("selection end") {
          auto const selectionStart = 2;
          auto const selectionLength = 3;
          givenSelection(controls, selectionStart, selectionLength); // 0 1 [2 3 4] 5 6 7
          auto const selectionEnd = selectionStart + selectionLength - 1;
          givenEnabled(controls, selectionEnd);

          auto const successor = selector.successor(selectionStart, false);

          CHECK_EQ(successor, selectionEnd);
        }
        SUBCASE("given step if sequencer is looping") {
          givenSelection(controls, 0, stepCount); // [0 1 2 3 4 5 6 7]
          auto constexpr step = 3;
          givenEnabled(controls, step); // The only enabled step

          auto const successor = selector.successor(step, true);

          CHECK_EQ(successor, step);
        }
      }

      SUBCASE("returns no step") {
        SUBCASE("if no selected step is enabled") {
          auto const selectionStart = 1;
          auto const selectionLength = 4;
          givenSelection(controls, selectionStart, selectionLength); // 0 [1 2 3 4] 5 6 7
          auto const onlyEnabledStep = 6;
          givenEnabled(controls, onlyEnabledStep); // Enabled but not in selection

          auto const successor = selector.successor(selectionStart, true);

          CHECK_LT(successor, 0);
        }

        SUBCASE("if selection wraps and no selected step is enabled") {
          auto const selectionStart = 5;
          auto const selectionLength = 7;
          givenSelection(controls, selectionStart, selectionLength); // 0 1 2 3] 4 [5 6 7
          auto const onlyEnabledStep = 4;                            // The only enabled step is not in the selection
          givenEnabled(controls, onlyEnabledStep);

          auto const successor = selector.successor(selectionStart, true);

          CHECK_LE(successor, 0);
        }

        SUBCASE("if given step is below selection") {
          givenSelection(controls, 4, 3); // 0 1 2 3 [4 5 6] 7
          auto const givenStep = 3;       // Lower than any selected step
          givenEnabled(controls, givenStep);

          auto const successor = selector.successor(givenStep, true);

          CHECK_LE(successor, 0);
        }

        SUBCASE("if given step is above selection") {
          givenSelection(controls, 4, 3); // 0 1 2 3 [4 5 6] 7
          auto const givenStep = 7;       // Higher than any selected step
          givenEnabled(controls, givenStep);

          auto const successor = selector.successor(givenStep, true);

          CHECK_LE(successor, 0);
        }

        SUBCASE("if given step not included in wrapped selection") {
          givenSelection(controls, 6, 4); // 0 1] 2 3 4 5 [6 7
          auto const givenStep = 5;       // Above selection end, below selection start
          givenEnabled(controls, givenStep);

          auto const successor = selector.successor(givenStep, true);

          CHECK_LE(successor, 0);
        }

        SUBCASE("if given step is selection and and sequencer is not looping") {
          auto constexpr step = 3;
          givenSelection(controls, 1, step); // 0 [1 2 3] 4 5 6 7

          auto const successor = selector.successor(step, false);

          CHECK_LT(successor, 0);
        }
      }
    }

    SUBCASE("first()") {
      SUBCASE("is selection start if start step is enabled") {
        auto constexpr selectionStart = 3;
        givenSelection(controls, selectionStart, 4);
        givenEnabled(controls, selectionStart);

        auto const first = selector.first();

        CHECK_EQ(first, selectionStart);
      }

      SUBCASE("is successor of selection start if start step is disabled") {
        auto constexpr selectionStart = 3;
        givenSelection(controls, selectionStart, 4); // Not enabled
        auto constexpr enabledSelectedStep = 5;
        givenEnabled(controls, enabledSelectedStep);

        auto const first = selector.first();
        auto const successorOfSelectionStart = selector.successor(selectionStart, false);

        CHECK_EQ(first, successorOfSelectionStart);
      }

      SUBCASE("is successor earlier in selection if sequencer is looping") {
        givenSelection(controls, 0, stepCount); // [0 1 2 3 4 5 6 7]
        auto constexpr step = 3;
        auto constexpr expectedSuccessor = step - 2; // Earlier in selection
        givenEnabled(controls, expectedSuccessor);

        auto const successor = selector.successor(step, true);

        CHECK_EQ(successor, expectedSuccessor);
      }

      SUBCASE("is no step if no selected step is enabled") {
        givenSelection(controls, 0, stepCount);                      // All selected...
        controls.isEnabled = [](int step) -> bool { return false; }; // ... but non enabled

        auto const first = selector.first();

        CHECK_LT(first, 0);
      }
    }
  }
} // namespace curve_sequencer_step_selector
} // namespace test
}