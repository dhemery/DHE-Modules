#include "stage/DeferMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class DeferModeTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(float, input, (), (const));
    MOCK_METHOD(void, showActive, (bool), ());
    MOCK_METHOD(void, output, (float), ());
  };

protected:
  NiceMock<Controls> controls{};

  dhe::stage::DeferMode<Controls> deferMode{controls};

  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
};

TEST_F(DeferModeTest, enter_showsStageActive) {
  EXPECT_CALL(controls, showActive(true));

  deferMode.enter();
}

TEST_F(DeferModeTest, exit_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  deferMode.exit();
}

TEST_F(DeferModeTest, execute_outputsInput) {
  auto constexpr input{5.34908F};

  givenInput(input);

  EXPECT_CALL(controls, output(input));

  deferMode.execute();
}
