#include "curve-sequencer/AdvancingMode.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{4};
static auto constexpr unusedLatch = dhe::Latch{};

using dhe::curve_sequencer::AdvancingMode;
using dhe::curve_sequencer::ModeId;
using Controls = dhe::curve_sequencer::CurveSequencerControls<stepCount>;

using ::testing::Test;

struct AdvancingModeTest : public Test {
  std::vector<rack::engine::Input> inputs{Controls::InputCount};
  std::vector<rack::engine::Param> params{Controls::ParameterCount};

  AdvancingMode<stepCount> advancingMode{inputs, params};

  void setEnabledButton(int step, bool state) { params[Controls::EnabledButtons + step].setValue(state ? 1.F : 0.F); }
  void setEnabledInput(int step, bool state) { inputs[Controls::EnabledInputs + step].setVoltage(state ? 10.F : 0.F); }
};

TEST_F(AdvancingModeTest, isNotTerminal) { ASSERT_EQ(advancingMode.isTerminal(), false); }

TEST_F(AdvancingModeTest, ifGivenStepEnabledByButton_nextStepIsGivenStep) {
  auto const step = 0;

  setEnabledButton(step, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, step, 0.F);

  ASSERT_EQ(next.step, step);
}

TEST_F(AdvancingModeTest, ifGivenStepEnabledByButton_nextModeIsGenerating) {
  auto const step = 1;

  setEnabledButton(step, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, step, 0.F);

  ASSERT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(AdvancingModeTest, ifGivenStepEnabledByInput_nextStepIsGivenStep) {
  auto const step = 2;

  setEnabledInput(step, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, step, 0.F);

  ASSERT_EQ(next.step, step);
}

TEST_F(AdvancingModeTest, ifGivenStepEnabledByInput_nextModeIsGenerating) {
  auto const step = 3;

  setEnabledInput(step, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, step, 0.F);

  ASSERT_EQ(next.modeId, ModeId::Generating);
}
