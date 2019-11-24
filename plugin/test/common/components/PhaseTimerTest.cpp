#include "components/PhaseTimer.h"

#include <gtest/gtest.h>

using dhe::PhaseTimer;

TEST(PhaseTimerTest, startsAtPhase0ByDefault) {
  PhaseTimer timer{};

  EXPECT_EQ(timer.phase(), 0.F);
}

TEST(PhaseTimerTest, startsInProgressByDefault) {
  PhaseTimer timer{};

  EXPECT_TRUE(timer.inProgress());
}

TEST(PhaseTimerTest, advance_addsDeltaToPhase) {
  PhaseTimer timer{};

  timer.advance(0.38F);

  EXPECT_EQ(timer.phase(), 0.38F);

  timer.advance(0.19F);

  EXPECT_EQ(timer.phase(), 0.57F);
}

TEST(PhaseTimerTest, isInProgress_ifPhaseIsLessThanOne) {
  PhaseTimer timer{0.9999F};

  EXPECT_TRUE(timer.inProgress());
}

TEST(PhaseTimerTest, isNotInProgress_ifPhaseIsAtLeastOne) {
  PhaseTimer timer{1.F};

  EXPECT_FALSE(timer.inProgress());
}

TEST(PhaseTimerTest, maximumPhaseIsOne) {
  PhaseTimer timer{1234.56789F};
  EXPECT_EQ(timer.phase(), 1.F);

  timer.advance(1234.56789F);
  EXPECT_EQ(timer.phase(), 1.F);
}

TEST(PhaseTimerTest, phaseIsZero_afterReset) {
  PhaseTimer timer{0.74F};

  timer.reset();

  EXPECT_EQ(timer.phase(), 0.F);
}

TEST(PhaseTimerTest, isInProgress_afterReset) {
  PhaseTimer timer{1.F};

  timer.reset();

  EXPECT_TRUE(timer.inProgress());
}
