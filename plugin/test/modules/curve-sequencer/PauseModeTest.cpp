#include "curve-sequencer/PauseMode.h"

#include "components/Latch.h"

#include <gtest/gtest.h>

using dhe::Latch;
using dhe::curve_sequencer::ModeId;
using dhe::curve_sequencer::PauseMode;

using ::testing::Test;

static auto constexpr risenLatch = Latch{true, true};
static auto constexpr fallenLatch = Latch{false, true};
static auto constexpr stableHighLatch = Latch{true, false};
static auto constexpr stableLowLatch = Latch{false, false};

class PauseModeTest : public Test {
protected:
  PauseMode pauseMode{};
};

TEST_F(PauseModeTest, isTerminal) {
  auto const pauseModeIsTerminal = pauseMode.isTerminal();

  ASSERT_EQ(pauseModeIsTerminal, true);
}

TEST_F(PauseModeTest, ifRunLatchRises_nextModeIsIdle) {
  auto const next = pauseMode.execute(risenLatch, Latch{}, -1, -2.F);

  ASSERT_EQ(next.modeId, ModeId::Idle);
}

TEST_F(PauseModeTest, ifRunLatchDoesNotRise_nextModeIsPaused) {
  auto next = pauseMode.execute(fallenLatch, Latch{}, -1, -2.F);
  EXPECT_EQ(next.modeId, ModeId::Paused);

  next = pauseMode.execute(stableHighLatch, Latch{}, -1, -2.F);
  EXPECT_EQ(next.modeId, ModeId::Paused);

  next = pauseMode.execute(stableLowLatch, Latch{}, -1, -2.F);
  EXPECT_EQ(next.modeId, ModeId::Paused);
}
