#include "modules/stage/InputMode.h"

#include "doctest/doctest.h"
#include "fake/FakeControls.h"

namespace test {
namespace stage {
  namespace input_mode {

    using dhe::stage::InputMode;

    TEST_CASE("stage::InputMode") {
      FakeControls controls{};
      InputMode<FakeControls> inputMode{controls};

      SUBCASE("enter deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        inputMode.enter();
        CHECK_FALSE(active);
      }

      SUBCASE("execute outputs input") {
        float input = 7.6344F;
        controls.input = [=]() -> float { return input; };

        float output{-99.F};
        controls.output = [&](float voltage) { output = voltage; };

        inputMode.execute();

        CHECK_EQ(output, input);
      }

      SUBCASE("exit deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        inputMode.exit();
        CHECK_FALSE(active);
      }
    }
  } // namespace input_mode
} // namespace stage
} // namespace test
