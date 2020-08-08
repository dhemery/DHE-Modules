#include "modules/stage/DeferMode.h"

#include "fake/FakeControls.h"

#include <doctest.h>

namespace test {
namespace stage {
  namespace defer_mode {

    using dhe::stage::DeferMode;

    TEST_CASE("stage::DeferMode") {
      FakeControls controls{};
      DeferMode<FakeControls> deferMode{controls};

      SUBCASE("enter activates stage") {
        auto active{false};
        controls.showActive = [&](bool b) { active = b; };
        deferMode.enter();
        CHECK(active);
      }

      SUBCASE("execute outputs input") {
        float input = 7.6344F;
        controls.input = [=]() -> float { return input; };

        float output{-99.F};
        controls.output = [&](float voltage) { output = voltage; };

        deferMode.execute();

        CHECK_EQ(output, input);
      }

      SUBCASE("exit deactivates stage") {
        auto active{true};
        controls.showActive = [&](bool b) { active = b; };
        deferMode.exit();
        CHECK_FALSE(active);
      }
    }
  } // namespace defer_mode
} // namespace stage
} // namespace test
