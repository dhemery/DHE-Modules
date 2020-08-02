#include "modules/stage/LevelMode.h"

#include "doctest/doctest.h"
#include "fake/FakeControls.h"

namespace test {
namespace stage {
  namespace level_mode {

    using dhe::stage::LevelMode;

    TEST_SUITE("stage::LevelMode") {
      FakeControls controls{};
      LevelMode<FakeControls> levelMode{controls};

      TEST_CASE("enter deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        levelMode.enter();
        CHECK_FALSE(active);
      }

      TEST_CASE("execute outputs input") {
        float level = 7.6344F;
        controls.level = [=]() -> float { return level; };

        float output{-99.F};
        controls.output = [&](float voltage) { output = voltage; };

        levelMode.execute();

        CHECK_EQ(output, level);
      }

      TEST_CASE("exit deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        levelMode.exit();
        CHECK_FALSE(active);
      }
    }
  } // namespace level_mode
} // namespace stage
} // namespace test
