#include "components/Latch.h"

#include <gtest/gtest.h>

using dhe::Latch;
using ::testing::Test;

TEST(LatchEqualityTest, equalIfSameStateAndEdge) {
  EXPECT_TRUE((Latch{false, false} == Latch{false, false}));
  EXPECT_TRUE((Latch{false, true} == Latch{false, true}));
  EXPECT_TRUE((Latch{true, false} == Latch{true, false}));
  EXPECT_TRUE((Latch{true, true} == Latch{true, true}));
}

TEST(LatchEqualityTest, unequalIfStatesDiffer) {
  EXPECT_FALSE((Latch{false, false} == Latch{true, false}));
  EXPECT_FALSE((Latch{false, true} == Latch{true, true}));
  EXPECT_FALSE((Latch{true, false} == Latch{false, false}));
  EXPECT_FALSE((Latch{true, true} == Latch{false, true}));
}

TEST(LatchEqualityTest, unequalIfEdgesDiffer) {
  EXPECT_FALSE((Latch{false, false} == Latch{false, true}));
  EXPECT_FALSE((Latch{false, true} == Latch{false, false}));
  EXPECT_FALSE((Latch{true, false} == Latch{true, true}));
  EXPECT_FALSE((Latch{true, true} == Latch{true, false}));
}

class LatchTest : public Test {
protected:
  Latch latch{};
};

TEST_F(LatchTest, defaultLatch_isLow) {
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isHigh(), false);
}

TEST_F(LatchTest, defaultLatch_isNotEdge) {
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

class LowLatch : public Test {
protected:
  Latch latch{false, false};
};

TEST_F(LowLatch, isUnchangedOnLowSignal) {
  latch.clock(false);

  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isHigh(), false);
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

TEST_F(LowLatch, risesOnHighSignal) {
  latch.clock(true);

  EXPECT_EQ(latch.isHigh(), true);
  EXPECT_EQ(latch.isLow(), false);
  EXPECT_EQ(latch.isEdge(), true);
  EXPECT_EQ(latch.isRise(), true);
  EXPECT_EQ(latch.isFall(), false);
}

class FallenLatch : public Test {
protected:
  Latch latch{false, true};
};

TEST_F(FallenLatch, losesEdgeOnLowSignal) {
  latch.clock(false);

  EXPECT_EQ(latch.isHigh(), false);
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

TEST_F(FallenLatch, risesOnHighSignal) {
  latch.clock(true);

  EXPECT_EQ(latch.isHigh(), true);
  EXPECT_EQ(latch.isLow(), false);
  EXPECT_EQ(latch.isEdge(), true);
  EXPECT_EQ(latch.isRise(), true);
  EXPECT_EQ(latch.isFall(), false);
}

class HighLatch : public Test {
protected:
  Latch latch{true, false};
};

TEST_F(HighLatch, fallsOnLowSignal) {
  latch.clock(false);

  EXPECT_EQ(latch.isHigh(), false);
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isEdge(), true);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), true);
}

TEST_F(HighLatch, isUnchangedOnHighSignal) {
  latch.clock(true);

  EXPECT_EQ(latch.isHigh(), true);
  EXPECT_EQ(latch.isLow(), false);
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

class RisenLatch : public Test {
protected:
  Latch latch{true, true};
};

TEST_F(RisenLatch, fallsOnLowSignal) {
  latch.clock(false);

  EXPECT_EQ(latch.isHigh(), false);
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isEdge(), true);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), true);
}

TEST_F(RisenLatch, losesEdgeOnHighSignal) {
  latch.clock(true);

  EXPECT_EQ(latch.isHigh(), true);
  EXPECT_EQ(latch.isLow(), false);
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

