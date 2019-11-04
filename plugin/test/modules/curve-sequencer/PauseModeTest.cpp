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

TEST_F(PauseModeTest, ifRunLatchRises_nextModeIsIdle) {
  auto const next = pauseMode.execute(risenLatch);

  ASSERT_EQ(next.mode, ModeId::Idle);
}

TEST_F(PauseModeTest, ifRunLatchDoesNotRise_nextModeIsPaused) {
  auto next = pauseMode.execute(fallenLatch);
  EXPECT_EQ(next.mode, ModeId::Paused);

  next = pauseMode.execute(stableHighLatch);
  EXPECT_EQ(next.mode, ModeId::Paused);

  next = pauseMode.execute(stableLowLatch);
  EXPECT_EQ(next.mode, ModeId::Paused);
}
