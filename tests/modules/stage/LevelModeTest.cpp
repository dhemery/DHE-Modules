#include "modules/stage/LevelMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class LevelModeTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(float, level, (), (const));
    MOCK_METHOD(void, showActive, (bool), ());
    MOCK_METHOD(void, output, (float), ());
  };

protected:
  NiceMock<Controls> controls{};

  dhe::stage::LevelMode<Controls> levelMode{controls};

  void givenLevel(float level) { ON_CALL(controls, level()).WillByDefault(Return(level)); }
};

TEST_F(LevelModeTest, enter_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  levelMode.enter();
}

TEST_F(LevelModeTest, exit_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  levelMode.exit();
}

TEST_F(LevelModeTest, execute_outputsLevel) {
  auto constexpr level{5.34908F};

  givenLevel(level);

  EXPECT_CALL(controls, output(level));

  levelMode.execute();
}
