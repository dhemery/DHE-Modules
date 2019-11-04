#include "curve-sequencer/Idle.h"

#include "components/Latch.h"

#include <curve-sequencer/CurveSequencerControls.h>
#include <engine/Param.hpp>
#include <gtest/gtest.h>

auto constexpr stepCount{4};
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;
using dhe::Latch;
using dhe::curve_sequencer::Idle;
using dhe::curve_sequencer::Mode;

using ::testing::Test;

static auto constexpr risenLatch = Latch{true, true};
static auto constexpr fallenLatch = Latch{false, true};
static auto constexpr stableHighLatch = Latch{true, false};
static auto constexpr stableLowLatch = Latch{false, false};

class IdleTest : public Test {
protected:
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  Idle<stepCount> idleMode{params};

  void givenStartStep(int step) { params[Controls::StartKnob].setValue(static_cast<float>(step)); }
};

TEST_F(IdleTest, ifGateLatchRises_returnsAdvancingFromStartStep) {
  auto startStep = 2;
  givenStartStep(startStep);

  auto next = idleMode.execute(risenLatch);

  EXPECT_EQ(next.mode, Mode::Advancing);
  EXPECT_EQ(next.step, startStep);
}

TEST_F(IdleTest, ifRunLatchIsHigh_andGateLatchDoesNotRise_nextModeIsIdle) {
  auto next = idleMode.execute(stableLowLatch);
  EXPECT_EQ(next.mode, Mode::Idle);

  next = idleMode.execute(stableHighLatch);
  EXPECT_EQ(next.mode, Mode::Idle);

  next = idleMode.execute(fallenLatch);
  EXPECT_EQ(next.mode, Mode::Idle);
}
