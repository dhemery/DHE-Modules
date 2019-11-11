#include "curve-sequencer/Idle.h"

#include "components/Latch.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::Idle;
using dhe::curve_sequencer::SequenceMode;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

static auto constexpr risenLatch = Latch{true, true};
static auto constexpr fallenLatch = Latch{false, true};
static auto constexpr stableHighLatch = Latch{true, false};
static auto constexpr stableLowLatch = Latch{false, false};

class MockControls {
public:
  MOCK_METHOD(int, selectionStart, (), (const));
};

class IdleTest : public Test {
protected:
  NiceMock<MockControls> controls{};
  Idle<MockControls> idle{controls};

  void givenSelectionStart(int step) { ON_CALL(controls, selectionStart()).WillByDefault(Return(step)); };
};

TEST_F(IdleTest, ifGateLatchRises_returnsAdvancingFromStartStep) {
  auto startStep = 2;
  givenSelectionStart(startStep);

  auto next = idle.execute(risenLatch);

  EXPECT_EQ(next.mode, SequenceMode::Advancing);
  EXPECT_EQ(next.step, startStep);
}

TEST_F(IdleTest, ifRunLatchIsHigh_andGateLatchDoesNotRise_nextModeIsIdle) {
  auto next = idle.execute(stableLowLatch);
  EXPECT_EQ(next.mode, SequenceMode::Idle);

  next = idle.execute(stableHighLatch);
  EXPECT_EQ(next.mode, SequenceMode::Idle);

  next = idle.execute(fallenLatch);
  EXPECT_EQ(next.mode, SequenceMode::Idle);
}
