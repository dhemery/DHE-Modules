#include "curve-sequencer/IdleMode.h"

#include "components/Latch.h"

#include <curve-sequencer/CurveSequencerControls.h>
#include <engine/Param.hpp>
#include <gtest/gtest.h>

auto constexpr stepCount{4};
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;
using dhe::Latch;
using dhe::curve_sequencer::IdleMode;
using dhe::curve_sequencer::ModeId;

using ::testing::Test;

static auto constexpr risenLatch = Latch{true, true};
static auto constexpr fallenLatch = Latch{false, true};
static auto constexpr stableHighLatch = Latch{true, false};
static auto constexpr stableLowLatch = Latch{false, false};

class IdleModeTest : public Test {
protected:
  std::vector<rack::engine::Param> params{Controls::ParameterCount};
  IdleMode<stepCount> idleMode{params};

  void givenStartStep(int step) { params[Controls::StartKnob].setValue(static_cast<float>(step)); }
};

TEST_F(IdleModeTest, isTerminal) { ASSERT_EQ(idleMode.isTerminal(), true); }

TEST_F(IdleModeTest, ifRunLatchFalls_nextModeIsPaused) {
  auto const next = idleMode.execute(fallenLatch, Latch{}, -1, -2.F);
  ASSERT_EQ(next.modeId, ModeId::Paused);
}

TEST_F(IdleModeTest, ifRunLatchIsHigh_andGateLatchRises_nextModeIsAdvancing) {
  auto next = idleMode.execute(stableHighLatch, risenLatch, -1, -2.F);
  ASSERT_EQ(next.modeId, ModeId::Advancing);
}

TEST_F(IdleModeTest, ifRunLatchIsHigh_andGateLatchRises_nextStepIsStartStep) {
  auto startStep = 2;
  givenStartStep(startStep);

  auto next = idleMode.execute(stableHighLatch, risenLatch, -1, -2.F);

  ASSERT_EQ(next.step, startStep);
}

TEST_F(IdleModeTest, ifRunLatchIsHigh_andGateLatchDoesNotRise_nextModeIsIdle) {
  auto next = idleMode.execute(stableHighLatch, stableLowLatch, -1, -2.F);
  ASSERT_EQ(next.modeId, ModeId::Idle);

  next = idleMode.execute(stableHighLatch, stableHighLatch, -1, -2.F);
  ASSERT_EQ(next.modeId, ModeId::Idle);

  next = idleMode.execute(stableHighLatch, fallenLatch, -1, -2.F);
  ASSERT_EQ(next.modeId, ModeId::Idle);
}
