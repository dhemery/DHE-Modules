#include <gtest/gtest.h>
#include <modules/components/Latch.h>

using dhe::Latch;
using ::testing::Test;

struct DefaultLatch : public Test {
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

struct LowLatch : public Test {
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

struct FallenLatch : public Test {
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

struct HighLatch : public Test {
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

struct RisenLatch : public Test {
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
