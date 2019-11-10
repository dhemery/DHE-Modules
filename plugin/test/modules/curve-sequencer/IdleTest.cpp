#include "curve-sequencer/Idle.h"

#include "components/Latch.h"

#include <curve-sequencer/CurveSequencerControls.h>
#include <engine/Param.hpp>
#include <gtest/gtest.h>

auto constexpr stepCount{4};
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;
using dhe::Latch;
using dhe::curve_sequencer::Idle;
using dhe::curve_sequencer::SequenceMode;

using ::testing::Test;

static auto constexpr risenLatch = Latch{true, true};
static auto constexpr fallenLatch = Latch{false, true};
static auto constexpr stableHighLatch = Latch{true, false};
static auto constexpr stableLowLatch = Latch{false, false};

class IdleTest : public Test {
protected:
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  Idle<stepCount> idle{params};

  void givenSelectionStart(int step) { params[Controls::SelectionStartKnob].setValue(static_cast<float>(step)); }
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
