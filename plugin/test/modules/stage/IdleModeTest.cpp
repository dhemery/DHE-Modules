#include "stage/IdleMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class IdleModeTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(float, input, (), (const));
    MOCK_METHOD(void, showActive, (bool), ());
    MOCK_METHOD(void, output, (float), ());
  };

protected:
  NiceMock<Controls> controls{};
  dhe::stage::IdleMode<Controls> idleMode{controls};
};

TEST_F(IdleModeTest, enter_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  idleMode.enter();
}

TEST_F(IdleModeTest, exit_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  idleMode.exit();
}

TEST_F(IdleModeTest, execute_outputsNothing) {
  EXPECT_CALL(controls, output(A<float>())).Times(0);

  idleMode.execute();
}
