#include "curve-sequencer/IdleMode.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"

#include <components/Latch.h>
#include <engine/Light.hpp>
#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::IdleMode;
using dhe::curve_sequencer::Mode;

using ::testing::Test;

static auto constexpr risenLatch = Latch{true, true};
static auto constexpr fallenLatch = Latch{false, true};
static auto constexpr stableHighLatch = Latch{true, false};
static auto constexpr stableLowLatch = Latch{false, false};

class IdleModeTest : public Test {
protected:
  IdleMode idleMode{};
};

TEST_F(IdleModeTest, isTerminal) {
  auto const pauseModeIsTerminal = idleMode.isTerminal();
  ASSERT_EQ(pauseModeIsTerminal, true);
}

TEST_F(IdleModeTest, ifRunLatchFalls_nextModeIsPaused) {
  auto const nextMode = idleMode.execute(fallenLatch, Latch{}, -1, -2.F);
  ASSERT_EQ(nextMode, Mode::Id::Paused);
}

TEST_F(IdleModeTest, ifRunLatchIsHigh_andGateLatchRises_nextModeIsAdvancing) {
  auto const nextMode = idleMode.execute(stableHighLatch, risenLatch, -1, -2.F);
  ASSERT_EQ(nextMode, Mode::Id::Advancing);
}

TEST_F(IdleModeTest, ifRunLatchIsHigh_andGateLatchDoesNotRise_nextModeIsIdle) {
  auto nextMode = idleMode.execute(stableHighLatch, stableLowLatch, -1, -2.F);
  ASSERT_EQ(nextMode, Mode::Id::Idle);

  nextMode = idleMode.execute(stableHighLatch, stableHighLatch, -1, -2.F);
  ASSERT_EQ(nextMode, Mode::Id::Idle);

  nextMode = idleMode.execute(stableHighLatch, fallenLatch, -1, -2.F);
  ASSERT_EQ(nextMode, Mode::Id::Idle);
}