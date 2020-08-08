#include "modules/stage/IdleMode.h"

#include "fake/FakeControls.h"

#include <doctest.h>

namespace test {
namespace stage {
  namespace idle_mode {

    using dhe::stage::IdleMode;

    TEST_CASE("stage::IdleMode") {
      FakeControls controls{};
      IdleMode<FakeControls> idleMode{controls};

      SUBCASE("enter deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        idleMode.enter();
        CHECK_FALSE(active);
      }

      SUBCASE("execute outputs input") {
        idleMode.execute(); // Will throw if any control func called
      }

      SUBCASE("exit deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        idleMode.exit();
        CHECK_FALSE(active);
      }
    }
  } // namespace idle_mode
} // namespace stage
} // namespace test
