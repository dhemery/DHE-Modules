#include <gtest/gtest.h>
#include <modules/components/FunctionLatch.h>

using dhe::FunctionLatch;

struct LatchTest : public ::testing::Test {
  bool input;
  FunctionLatch latch{[this]() -> bool { return input; }};
};

struct NewFunctionLatch : public LatchTest {};

TEST_F(NewFunctionLatch, isLow) {
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isHigh(), false);
}

TEST_F(NewFunctionLatch, isNotEdge) {
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

struct LowFunctionLatch : public LatchTest {
  void SetUp() override { latch.set(false, false); }
};

TEST_F(LowFunctionLatch, isUnchangedOnLowInput) {
  input = false;

  latch.step();

  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isHigh(), false);
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

TEST_F(LowFunctionLatch, risesOnHighInput) {
  input = true;

  latch.step();

  EXPECT_EQ(latch.isHigh(), true);
  EXPECT_EQ(latch.isLow(), false);
  EXPECT_EQ(latch.isEdge(), true);
  EXPECT_EQ(latch.isRise(), true);
  EXPECT_EQ(latch.isFall(), false);
}

TEST_F(LowFunctionLatch, set) {
  auto const newState = !latch.isHigh();
  auto const newEdge = !latch.isEdge();

  latch.set(newState, newEdge);

  EXPECT_EQ(latch.isHigh(), newState);
  EXPECT_EQ(latch.isEdge(), newEdge);
}

struct FallenFunctionLatch : public LatchTest {
  void SetUp() override { latch.set(false, true); }
};

TEST_F(FallenFunctionLatch, losesEdgeOnLowInput) {
  input = false;

  latch.step();

  EXPECT_EQ(latch.isHigh(), false);
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

TEST_F(FallenFunctionLatch, risesOnHighInput) {
  input = true;

  latch.step();

  EXPECT_EQ(latch.isHigh(), true);
  EXPECT_EQ(latch.isLow(), false);
  EXPECT_EQ(latch.isEdge(), true);
  EXPECT_EQ(latch.isRise(), true);
  EXPECT_EQ(latch.isFall(), false);
}

TEST_F(FallenFunctionLatch, set) {
  auto const newState = !latch.isHigh();
  auto const newEdge = !latch.isEdge();

  latch.set(newState, newEdge);

  EXPECT_EQ(latch.isHigh(), newState);
  EXPECT_EQ(latch.isEdge(), newEdge);
}

struct HighFunctionLatch : public LatchTest {
  void SetUp() override { latch.set(true, false); }
};

TEST_F(HighFunctionLatch, fallsOnLowInput) {
  input = false;

  latch.step();

  EXPECT_EQ(latch.isHigh(), false);
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isEdge(), true);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), true);
}

TEST_F(HighFunctionLatch, isUnchangedOnHighInput) {
  input = true;

  latch.step();

  EXPECT_EQ(latch.isHigh(), true);
  EXPECT_EQ(latch.isLow(), false);
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

TEST_F(HighFunctionLatch, set) {
  auto const newState = !latch.isHigh();
  auto const newEdge = !latch.isEdge();

  latch.set(newState, newEdge);

  EXPECT_EQ(latch.isHigh(), newState);
  EXPECT_EQ(latch.isEdge(), newEdge);
}

struct RisenFunctionLatch : public LatchTest {
  void SetUp() override { latch.set(true, true); }
};

TEST_F(RisenFunctionLatch, fallsOnLowInput) {
  input = false;

  latch.step();

  EXPECT_EQ(latch.isHigh(), false);
  EXPECT_EQ(latch.isLow(), true);
  EXPECT_EQ(latch.isEdge(), true);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), true);
}

TEST_F(RisenFunctionLatch, losesEdgeOnHighInput) {
  input = true;

  latch.step();

  EXPECT_EQ(latch.isHigh(), true);
  EXPECT_EQ(latch.isLow(), false);
  EXPECT_EQ(latch.isEdge(), false);
  EXPECT_EQ(latch.isRise(), false);
  EXPECT_EQ(latch.isFall(), false);
}

TEST_F(RisenFunctionLatch, set) {
  auto const newState = !latch.isHigh();
  auto const newEdge = !latch.isEdge();

  latch.set(newState, newEdge);

  EXPECT_EQ(latch.isHigh(), newState);
  EXPECT_EQ(latch.isEdge(), newEdge);
}
