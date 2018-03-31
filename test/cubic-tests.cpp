#include <lib/catch.hpp>
#include <modules/cubic-module.h>
#include <lib/matchers.h>

TEST_CASE("Cubic Module") {
  DHE::CubicModule module{};
  auto &offset_knob = module.params[module.OFFSET_KNOB].value;
  auto &scale_knob = module.params[module.SCALE_KNOB].value;
  auto &squared_knob = module.params[module.SQUARED_KNOB].value;
  auto &cubed_knob = module.params[module.CUBED_KNOB].value;

  auto &x = module.inputs[module.X].value;
  auto &y = module.outputs[module.Y].value;

  cubed_knob = 0.5f;  // default a = 0, so ax^3 = 0
  squared_knob = 0.5f;  // default b = 0, so bx^2 = 0
  scale_knob = 0.75f;   // default c = 1, so cx^1 = x
  offset_knob = 0.5f;   // default d = 0, so dx_0 = 0

  SECTION("offset") {
    x = 1.234f;
    SECTION("100% rotation adds bipolar max") {
      offset_knob = 1.f;
      module.step();
      REQUIRE_THAT(y, near(x + DHE::BIPOLAR_SIGNAL_RANGE.upper_bound));
    }

    SECTION("50% rotation adds 0") {
      offset_knob = 0.5f;
      module.step();
      REQUIRE_THAT(y, near(x + 0.f));
    }

    SECTION("0% rotation adds bipolar min") {
      offset_knob = 0.f;
      module.step();
      REQUIRE_THAT(y, near(x + DHE::BIPOLAR_SIGNAL_RANGE.lower_bound));
    }
  }

  SECTION("scale") {
    SECTION("100% rotation adds 2x") {
      x = 2.181f;
      scale_knob = 1.f;
      module.step();
      REQUIRE_THAT(y, near(x*2.f));
    }

    SECTION("75% rotation adds x") {
      x = 3.345;
      scale_knob = 0.75f;
      module.step();
      REQUIRE_THAT(y, near(x));
    }

    SECTION("50% rotation adds 0") {
      x = 2.847;
      scale_knob = 0.5f;
      module.step();
      REQUIRE_THAT(y, near(0.f));
    }

    SECTION("25% rotation adds -x") {
      x = 4.394;
      scale_knob = 0.25f;
      module.step();
      REQUIRE_THAT(y, near(-x));
    }

    SECTION("0% rotation adds -2x") {
      x = 4.394;
      scale_knob = 0.f;
      module.step();
      REQUIRE_THAT(y, near(x*-2.f));
    }
  }

  SECTION("squared") {
    SECTION("100% rotation adds 2x^2") {
      auto phase = 0.412f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      squared_knob = 1.f;
      module.step();
      auto x_squared = phase*phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      REQUIRE_THAT(y, near(x + 2.f*x_squared));
    }

    SECTION("75% rotation adds x^2") {
      auto phase = 0.573f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      squared_knob = 0.75f;
      module.step();
      auto x_squared = phase*phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      REQUIRE_THAT(y, near(x + x_squared));
    }

    SECTION("50% rotation adds 0") {
      auto phase = 0.633f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      squared_knob = 0.5f;
      module.step();
      REQUIRE_THAT(y, near(x + 0.f));
    }

    SECTION("25% rotation adds -x^2") {
      auto phase = 0.722f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      squared_knob = 0.25f;
      module.step();
      auto x_squared = phase*phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      REQUIRE_THAT(y, near(x - x_squared, 5));
    }

    SECTION("0% rotation adds -2x^2") {
      auto phase = 0.140f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      squared_knob = 0.f;
      module.step();
      auto x_squared = phase*phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      REQUIRE_THAT(y, near(x - 2.f*x_squared));
    }
  }

  SECTION("cubed") {
    SECTION("100% rotation adds 2x^3") {
      auto phase = 0.583f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      cubed_knob = 1.f;
      module.step();
      auto x_cubed = phase*phase*phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      REQUIRE_THAT(y, near(x + 2.f*x_cubed));
    }

    SECTION("75% rotation adds x^3") {
      auto phase = 0.653f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      cubed_knob = 0.75f;
      module.step();
      auto x_cubed = phase*phase*phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      REQUIRE_THAT(y, near(x + x_cubed));
    }

    SECTION("50% rotation adds 0") {
      auto phase = 0.345f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      cubed_knob = 0.5f;
      module.step();
      REQUIRE_THAT(y, near(x + 0.f));
    }

    SECTION("25% rotation adds -x^3") {
      auto phase = 0.822f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      cubed_knob = 0.25f;
      module.step();
      auto x_cubed = phase*phase*phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      REQUIRE_THAT(y, near(x - x_cubed));
    }

    SECTION("0% rotation adds -2x^3") {
      auto phase = 0.102f;
      x = phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      cubed_knob = 0.f;
      module.step();
      auto x_cubed = phase*phase*phase*DHE::BIPOLAR_SIGNAL_RANGE.upper_bound;
      REQUIRE_THAT(y, near(x - 2.f*x_cubed));
    }
  }
}