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
    auto const successorOfSelectionStart =
        selector.successor(selectionStart, false);

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
    givenSelection(controls, 0, stepCount); // All selected...
    controls.isEnabled = [](int step) -> bool {
      return false;
    }; // ... but non enabled

    auto const first = selector.first();

    CHECK_LT(first, 0);
  }
}
