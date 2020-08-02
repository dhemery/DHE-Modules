#include "modules/stage/DeferMode.h"

#include "doctest/doctest.h"
#include "fake/FakeControls.h"

namespace test {
namespace stage {

  using dhe::stage::DeferMode;

  TEST_SUITE("stage::DeferMode") {
    FakeControls controls{};
    DeferMode<FakeControls> deferMode{controls};

    TEST_CASE("enter activates stage") {
      auto active{false};
      controls.showActive = [&](bool b) { active = b; };
      deferMode.enter();
      CHECK(active);
    }

    TEST_CASE("execute outputs input") {
      float input = 7.6344F;
      controls.input = [=]() -> float { return input; };

      float output{-99.F};
      controls.output = [&](float voltage) { output = voltage; };

      deferMode.execute();

      CHECK_EQ(input, output);
    }

    TEST_CASE("exit deactivates stage") {
      auto active{true};
      controls.showActive = [&](bool b) { active = b; };
      deferMode.exit();
      CHECK_FALSE(active);
    }
  }
} // namespace stage
} // namespace test
