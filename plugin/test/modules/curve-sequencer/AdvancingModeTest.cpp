#include "curve-sequencer/AdvancingMode.h"

#include "components/Latch.h"
#include "curve-sequencer/CurveSequencerControls.h"

#include <engine/Param.hpp>
#include <engine/Port.hpp>
#include <gtest/gtest.h>

static auto constexpr stepCount{8};
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
  void setSelection(int start, int length) {
    params[Controls::StartKnob].setValue(static_cast<float>(start));
    params[Controls::StepsKnob].setValue(static_cast<float>(length));
  }
};

TEST_F(AdvancingModeTest, isNotTerminal) { ASSERT_EQ(advancingMode.isTerminal(), false); }

TEST_F(AdvancingModeTest, detectsStepEnabledByButton) {
  auto const step = 4;

  setSelection(0, stepCount);
  setEnabledButton(step, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, step, 0.F);

  ASSERT_EQ(next.step, step);
}

TEST_F(AdvancingModeTest, detectsStepEnabledByInput) {
  auto const step = 4;

  setSelection(0, stepCount);
  setEnabledInput(step, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, step, 0.F);

  ASSERT_EQ(next.step, step);
}

TEST_F(AdvancingModeTest, ifGivenStepIsEnabled_returnsGenerateGivenStep) {
  auto const step = 0;

  setSelection(0, stepCount);
  setEnabledButton(step, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, step, 0.F);

  EXPECT_EQ(next.step, step);
  EXPECT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(AdvancingModeTest, ifFirstEnabledStepInSelectionIsAboveGivenStep_returnsGeneratingEnabledStep) {
  auto const startStep = 2;
  auto const firstEnabledStep = startStep + 2;

  setSelection(0, stepCount);
  setEnabledButton(firstEnabledStep, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, startStep, 0.F);

  EXPECT_EQ(next.step, firstEnabledStep);
  EXPECT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(AdvancingModeTest, ifFirstEnabledStepIsLastStepInSelection_returnsGeneratingEnabledStep) {
  auto const selectionStart = 2;
  auto const selectionLength = 3;
  auto const lastStepInSelection = selectionStart + selectionLength - 1;

  setSelection(selectionStart, selectionLength);
  setEnabledButton(lastStepInSelection, true);

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, selectionStart, 0.F);

  EXPECT_EQ(next.step, lastStepInSelection);
  EXPECT_EQ(next.modeId, ModeId::Generating);
}

TEST_F(AdvancingModeTest, ifNoSelectedStepIsEnabled_returnsIdle) {
  auto const selectionStart = 1;
  auto const selectionLength = 4;
  setSelection(selectionStart, selectionLength);
  setEnabledButton(selectionStart + selectionLength, true); // Enabled but not in selection

  auto const next = advancingMode.execute(unusedLatch, unusedLatch, 0, 0.F);

  EXPECT_EQ(next.modeId, ModeId::Idle);
}

// TODO: 6 012+]4[567 finds enabled step by wrapping
// TODO: 5 0123]4[567 idle if no step enabled after wrapping
// TODO: 1 0+23[456]7 idle if given step below selection
// TODO: 6 0[123]45+7 idle if given step above selection
// TODO: 4 01]23+5[67 idle if given step between selection
