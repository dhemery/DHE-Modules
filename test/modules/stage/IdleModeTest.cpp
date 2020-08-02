#include "modules/stage/IdleMode.h"

#include "doctest/doctest.h"
#include "fake/FakeControls.h"

namespace test {
namespace stage {
  namespace idle_mode {

    using dhe::stage::IdleMode;

    TEST_SUITE("stage::IdleMode") {
      FakeControls controls{};
      IdleMode<FakeControls> idleMode{controls};

      TEST_CASE("enter deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        idleMode.enter();
        CHECK_FALSE(active);
      }

      TEST_CASE("execute outputs input") {
        idleMode.execute(); // Will throw if any control func called
      }

      TEST_CASE("exit deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        idleMode.exit();
        CHECK_FALSE(active);
      }
    }
  } // namespace idle_mode
} // namespace stage
} // namespace test
