#include "modules/stage/LevelMode.h"

#include "fake/FakeControls.h"

#include <doctest.h>

namespace test {
namespace stage {
namespace level_mode {

using dhe::stage::LevelMode;

TEST_CASE("stage::LevelMode") {
  FakeControls controls{};
  LevelMode<FakeControls> levelMode{controls};

  SUBCASE("enter deactivates stage") {
    auto active{true};
    controls.showActive = [&](bool b) { active = b; };
    levelMode.enter();
    CHECK_FALSE(active);
  }

  SUBCASE("execute outputs input") {
    float level = 7.6344F;
    controls.level = [=]() -> float { return level; };

    float output{-99.F};
    controls.output = [&](float voltage) { output = voltage; };

    levelMode.execute();

    CHECK_EQ(output, level);
  }

  SUBCASE("exit deactivates stage") {
    auto active{true};
    controls.showActive = [&](bool b) { active = b; };
    levelMode.exit();
    CHECK_FALSE(active);
  }
}
} // namespace level_mode
} // namespace stage
} // namespace test
