#include "modules/components/Ramp.h"

#include <gtest/gtest.h>

using dhe::Ramp;
using ::testing::Test;

class RampTest : public Test {
protected:
  Ramp ramp{};
};

TEST_F(RampTest, newRamp_phaseIsZero) { EXPECT_EQ(ramp.phase(), 0.F); }

TEST_F(RampTest, newRamp_isIncomplete) { EXPECT_FALSE(ramp.isComplete()); }

TEST_F(RampTest, advance_addsDeltaToPhase) {
  ramp.advance(0.38F);

  EXPECT_EQ(ramp.phase(), 0.38F);

  ramp.advance(0.19F);

  EXPECT_EQ(ramp.phase(), 0.57F);
}

TEST_F(RampTest, isIncomplete_ifPhaseIsLessThan1) {
  ramp.advance(0.9999F);

  EXPECT_FALSE(ramp.isComplete());
}

TEST_F(RampTest, isComplete_ifPhaseIsAtLeast1) {
  ramp.advance(1.F);

  EXPECT_TRUE(ramp.isComplete());
}
