#include "components/OneShotPhaseAccumulator.h"

#include <gtest/gtest.h>

using dhe::OneShotPhaseAccumulator;
using State = dhe::OneShotPhaseAccumulator::State;

TEST(OneShotPhaseAccumulatorTest, defaultRamp_phaseIsZero) {
  OneShotPhaseAccumulator accumulator{};

  EXPECT_EQ(accumulator.phase(), 0.F);
}

TEST(OneShotPhaseAccumulatorTest, defaultRamp_isIncomplete) {
  OneShotPhaseAccumulator accumulator{};

  EXPECT_EQ(accumulator.state(), State::Incomplete);
}

TEST(OneShotPhaseAccumulatorTest, advance_addsDeltaToPhase) {
  OneShotPhaseAccumulator accumulator{};

  accumulator.advance(0.38F);

  EXPECT_EQ(accumulator.phase(), 0.38F);

  accumulator.advance(0.19F);

  EXPECT_EQ(accumulator.phase(), 0.57F);
}

TEST(OneShotPhaseAccumulatorTest, isIncomplete_ifPhaseIsLessThanOne) {
  OneShotPhaseAccumulator accumulator{};

  accumulator.advance(0.9999F);

  EXPECT_EQ(accumulator.state(), State::Incomplete);
}

TEST(OneShotPhaseAccumulatorTest, isComplete_ifPhaseIsAtLeastOne) {
  OneShotPhaseAccumulator accumulator{};

  accumulator.advance(1.F);

  EXPECT_EQ(accumulator.state(), State::Complete);
}

TEST(OneShotPhaseAccumulatorTest, maximumPhaseIsOne) {
  OneShotPhaseAccumulator accumulator{};

  accumulator.advance(1234.56789F);

  EXPECT_EQ(accumulator.phase(), 1.F);
}

TEST(OneShotPhaseAccumulatorTest, phaseIsZero_afterReset) {
  OneShotPhaseAccumulator accumulator{};

  accumulator.advance(0.74F);

  accumulator.reset();

  EXPECT_EQ(accumulator.phase(), 0.F);
}

TEST(OneShotPhaseAccumulatorTest, isIncomplete_afterReset) {
  OneShotPhaseAccumulator accumulator{};

  accumulator.advance(999.F);
  EXPECT_EQ(accumulator.state(), State::Complete);

  accumulator.reset();

  EXPECT_EQ(accumulator.state(), State::Incomplete);
}
