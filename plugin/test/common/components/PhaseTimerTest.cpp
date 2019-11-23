#include "components/PhaseTimer.h"

#include <gtest/gtest.h>

using dhe::PhaseTimer;

TEST(PhaseTimerTest, defaultRamp_phaseIsZero) {
  PhaseTimer timer{};

  EXPECT_EQ(timer.phase(), 0.F);
}

TEST(PhaseTimerTest, defaultRamp_isNotExpired) {
  PhaseTimer timer{};

  EXPECT_FALSE(timer.isExpired());
}

TEST(PhaseTimerTest, operatorPlusEquals_addsDeltaToPhase) {
  PhaseTimer timer{};

  timer.advance(0.38F);

  EXPECT_EQ(timer.phase(), 0.38F);

  timer.advance(0.19F);

  EXPECT_EQ(timer.phase(), 0.57F);
}

TEST(PhaseTimerTest, isNotExpired_ifPhaseIsLessThanOne) {
  PhaseTimer timer{0.9999F};

  EXPECT_FALSE(timer.isExpired());
}

TEST(PhaseTimerTest, isExpired_ifPhaseIsAtLeastOne) {
  PhaseTimer timer{1.F};

  EXPECT_TRUE(timer.isExpired());
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

TEST(PhaseTimerTest, isNotExpired_afterReset) {
  PhaseTimer timer{1.F};

  timer.reset();

  EXPECT_FALSE(timer.isExpired());
}
