#include "modules/components/Latch.h"

#include <gtest/gtest.h>

struct DefaultLatch : public ::testing::Test {
  dhe::Latch latch{};
};

TEST_F(DefaultLatch, isNotHigh) { EXPECT_EQ(latch.high(), false); }

TEST_F(DefaultLatch, isNotEdge) { EXPECT_EQ(latch.edge(), false); }

struct LatchLowNotEdge : public ::testing::Test {
  dhe::Latch latch{false, false};
};

TEST_F(LatchLowNotEdge, stepFalse) {
  latch.step(false);

  EXPECT_EQ(latch.high(), false);
  EXPECT_EQ(latch.edge(), false);
}

TEST_F(LatchLowNotEdge, stepTrue) {
  latch.step(true);

  EXPECT_EQ(latch.high(), true);
  EXPECT_EQ(latch.edge(), true);
}

TEST_F(LatchLowNotEdge, set) {
  latch.set();

  EXPECT_EQ(latch.high(), true);
  EXPECT_EQ(latch.edge(), false);
}

TEST_F(LatchLowNotEdge, reset) {
  latch.reset();

  EXPECT_EQ(latch.high(), false);
  EXPECT_EQ(latch.edge(), false);
}

struct LatchLowOnEdge : public ::testing::Test {
  dhe::Latch latch{false, true};
};

TEST_F(LatchLowOnEdge, stepFalse) {
  latch.step(false);

  EXPECT_EQ(latch.high(), false);
  EXPECT_EQ(latch.edge(), false);
}

TEST_F(LatchLowOnEdge, stepTrue) {
  latch.step(true);

  EXPECT_EQ(latch.high(), true);
  EXPECT_EQ(latch.edge(), true);
}

TEST_F(LatchLowOnEdge, set) {
  latch.set();

  EXPECT_EQ(latch.high(), true);
  EXPECT_EQ(latch.edge(), false);
}

TEST_F(LatchLowOnEdge, reset) {
  latch.reset();

  EXPECT_EQ(latch.high(), false);
  EXPECT_EQ(latch.edge(), false);
}

struct LatchHighNotEdge : public ::testing::Test {
  dhe::Latch latch{true, false};
};

TEST_F(LatchHighNotEdge, stepFalse) {
  latch.step(false);

  EXPECT_EQ(latch.high(), false);
  EXPECT_EQ(latch.edge(), true);
}

TEST_F(LatchHighNotEdge, stepTrue) {
  latch.step(true);

  EXPECT_EQ(latch.high(), true);
  EXPECT_EQ(latch.edge(), false);
}

TEST_F(LatchHighNotEdge, set) {
  latch.set();

  EXPECT_EQ(latch.high(), true);
  EXPECT_EQ(latch.edge(), false);
}

TEST_F(LatchHighNotEdge, reset) {
  latch.reset();

  EXPECT_EQ(latch.high(), false);
  EXPECT_EQ(latch.edge(), false);
}

struct LatchHighOnEdge : public ::testing::Test {
  dhe::Latch latch{true, true};
};

TEST_F(LatchHighOnEdge, stepFalse) {
  latch.step(false);

  EXPECT_EQ(latch.high(), false);
  EXPECT_EQ(latch.edge(), true);
}

TEST_F(LatchHighOnEdge, stepTrue) {
  latch.step(true);

  EXPECT_EQ(latch.high(), true);
  EXPECT_EQ(latch.edge(), false);
}

TEST_F(LatchHighOnEdge, set) {
  latch.set();

  EXPECT_EQ(latch.high(), true);
  EXPECT_EQ(latch.edge(), false);
}

TEST_F(LatchHighOnEdge, reset) {
  latch.reset();

  EXPECT_EQ(latch.high(), false);
  EXPECT_EQ(latch.edge(), false);
}
