#include "modules/stage/input-mode.h"

#include "fake/FakeControls.h"

#include <doctest.h>

namespace test {
namespace stage {
namespace input_mode {

using dhe::stage::InputMode;

TEST_CASE("stage::InputMode") {
  FakeControls controls{};
  InputMode<FakeControls> input_mode{controls};

  SUBCASE("enter deactivates stage") {
    auto active{true};
    controls.show_active = [&](bool b) { active = b; };
    input_mode.enter();
    CHECK_FALSE(active);
  }

  SUBCASE("execute outputs input") {
    float input = 7.6344F;
    controls.input = [=]() -> float { return input; };

    float output{-99.F};
    controls.output = [&](float voltage) { output = voltage; };

    input_mode.execute();

    CHECK_EQ(output, input);
  }

  SUBCASE("exit deactivates stage") {
    auto active{true};
    controls.show_active = [&](bool b) { active = b; };
    input_mode.exit();
    CHECK_FALSE(active);
  }
}
} // namespace input_mode
} // namespace stage
} // namespace test
