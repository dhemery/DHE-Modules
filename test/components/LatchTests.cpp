#include "modules/components/Latch.h"

#include <gtest/gtest.h>

using dhe::Latch;
using ::testing::Test;

class DefaultLatch : public Test {
protected:
  Latch latch{};
};

TEST_F(DefaultLatch, isLow) {
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isHigh(), false);
}

TEST_F(DefaultLatch, isNotEdge) {
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
