#include "modules/stage/InputMode.h"

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::A;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class InputModeTest : public Test {
  class Controls {
  public:
    MOCK_METHOD(float, input, (), (const));
    MOCK_METHOD(void, showActive, (bool), ());
    MOCK_METHOD(void, output, (float), ());
  };

protected:
  NiceMock<Controls> controls{};

  dhe::stage::InputMode<Controls> inputMode{controls};

  void givenInput(float input) { ON_CALL(controls, input()).WillByDefault(Return(input)); }
};

TEST_F(InputModeTest, enter_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  inputMode.enter();
}

TEST_F(InputModeTest, exit_showsStageInactive) {
  EXPECT_CALL(controls, showActive(false));

  inputMode.exit();
}

TEST_F(InputModeTest, execute_outputsInput) {
  auto constexpr input{5.34908F};

  givenInput(input);

  EXPECT_CALL(controls, output(input));

  inputMode.execute();
}
