#include "modules/curve-sequencer/Step.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

struct FakeModule {
  MOCK_METHOD(void, setGenerating, (int, bool) );
};

using Step = dhe::curve_sequencer::Step<FakeModule>;

struct WhileIdle : public ::testing::Test {
  int stepIndex = 3;
  FakeModule module;
  Step step{module, stepIndex};
};

TEST_F(WhileIdle, startSetsGeneratingTrue) {
  EXPECT_CALL(module, setGenerating(stepIndex, true));

  step.start();
}
