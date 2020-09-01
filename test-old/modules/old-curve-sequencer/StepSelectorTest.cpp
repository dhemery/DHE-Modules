#include "modules/old-curve-sequencer/step-selector.h"

#include "fake/Controls.h"

#include <doctest.h>

namespace test {
namespace old_curve_sequencer {
namespace step_selector {
static auto constexpr stepCount{8};
using dhe::old_curve_sequencer::StepSelector;

static inline void givenEnabled(fake::Controls &controls, int enabledStep) {
  controls.is_enabled = [=](int step) -> bool { return step == enabledStep; };
}
static inline void givenSelection(fake::Controls &controls, int start,
                                  int length) {
  controls.selection_start = [=]() -> int { return start; };
  controls.selection_length = [=]() -> int { return length; };
}

TEST_CASE("old_curve_sequencer::StepSelector") {
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
        auto const selection_start = 6;
        auto const selection_length = 4;
        givenSelection(controls, selection_start,
                       selection_length);  // 0 1] 2 3 4 5 [6 7
        auto const expectedSuccessor = 1; // Must wrap to find
        givenEnabled(controls, expectedSuccessor);

        auto const successor = selector.successor(selection_start, false);

        CHECK_EQ(successor, expectedSuccessor);
      }

      SUBCASE("selection end") {
        auto const selection_start = 2;
        auto const selection_length = 3;
        givenSelection(controls, selection_start,
                       selection_length); // 0 1 [2 3 4] 5 6 7
        auto const selectionEnd = selection_start + selection_length - 1;
        givenEnabled(controls, selectionEnd);

        auto const successor = selector.successor(selection_start, false);

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
        auto const selection_start = 1;
        auto const selection_length = 4;
        givenSelection(controls, selection_start,
                       selection_length); // 0 [1 2 3 4] 5 6 7
        auto const onlyEnabledStep = 6;
        givenEnabled(controls, onlyEnabledStep); // Enabled but not in selection

        auto const successor = selector.successor(selection_start, true);

        CHECK_LT(successor, 0);
      }

      SUBCASE("if selection wraps and no selected step is enabled") {
        auto const selection_start = 5;
        auto const selection_length = 7;
        givenSelection(controls, selection_start,
                       selection_length); // 0 1 2 3] 4 [5 6 7
        auto const only_enabled_step =
            4; // The only enabled step is not in the selection
        givenEnabled(controls, only_enabled_step);

        auto const successor = selector.successor(selection_start, true);

        CHECK_LE(successor, 0);
      }

      SUBCASE("if given step is below selection") {
        givenSelection(controls, 4, 3); // 0 1 2 3 [4 5 6] 7
        auto const given_step = 3;       // Lower than any selected step
        givenEnabled(controls, given_step);

        auto const successor = selector.successor(given_step, true);

        CHECK_LE(successor, 0);
      }

      SUBCASE("if given step is above selection") {
        givenSelection(controls, 4, 3); // 0 1 2 3 [4 5 6] 7
        auto const given_step = 7;       // Higher than any selected step
        givenEnabled(controls, given_step);

        auto const successor = selector.successor(given_step, true);

        CHECK_LE(successor, 0);
      }

      SUBCASE("if given step not included in wrapped selection") {
        givenSelection(controls, 6, 4); // 0 1] 2 3 4 5 [6 7
        auto const given_step = 5; // Above selection end, below selection start
        givenEnabled(controls, given_step);

        auto const successor = selector.successor(given_step, true);

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
      auto constexpr selection_start = 3;
      givenSelection(controls, selection_start, 4);
      givenEnabled(controls, selection_start);

      auto const first = selector.first();

      CHECK_EQ(first, selection_start);
    }

    SUBCASE("is successor of selection start if start step is disabled") {
      auto constexpr selection_start = 3;
      givenSelection(controls, selection_start, 4); // Not enabled
      auto constexpr enabled_selected_step = 5;
      givenEnabled(controls, enabled_selected_step);

      auto const first = selector.first();
      auto const successor_of_selection_start =
          selector.successor(selection_start, false);

      CHECK_EQ(first, successor_of_selection_start);
    }

    SUBCASE("is successor earlier in selection if sequencer is looping") {
      givenSelection(controls, 0, stepCount); // [0 1 2 3 4 5 6 7]
      auto constexpr step = 3;
      auto constexpr expected_successor = step - 2; // Earlier in selection
      givenEnabled(controls, expected_successor);

      auto const successor = selector.successor(step, true);

      CHECK_EQ(successor, expected_successor);
    }

    SUBCASE("is no step if no selected step is enabled") {
      givenSelection(controls, 0, stepCount); // All selected...
      controls.is_enabled = [](int step) -> bool {
        return false;
      }; // ... but non enabled

      auto const first = selector.first();

      CHECK_LT(first, 0);
    }
  }
}
} // namespace step_selector
} // namespace curve_sequencer
} // namespace test
