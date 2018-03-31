#include <lib/catch.hpp>
#include <modules/cubic-module.h>
#include <lib/matchers.h>

TEST_CASE("Cubic Module") {
  DHE::CubicModule module{};
  auto &offset_knob = module.params[module.OFFSET_KNOB].value;
  auto &scale_knob = module.params[module.SCALE_KNOB].value;
  auto &squared_knob = module.params[module.SQUARED_KNOB].value;

  auto &x = module.inputs[module.X].value;
  auto &y = module.outputs[module.Y].value;

  squared_knob = 0.5f;  // default b = 0, so bx^2 = 0
  scale_knob = 0.75f;   // default c = 1, so cx^1 = x
  offset_knob = 0.5f;   // default d = 0, so dx_0 = 0

  SECTION("offset") {
    x = 1.234f;
    SECTION("max rotation adds max bipolar signal") {
      offset_knob = 1.f;
      module.step();
      REQUIRE_THAT(y, near(x + DHE::BIPOLAR_SIGNAL_RANGE.upper_bound));
    }

    SECTION("min rotation adds min bipolar signal") {
      offset_knob = 0.f;
      module.step();
      REQUIRE_THAT(y, near(x + DHE::BIPOLAR_SIGNAL_RANGE.lower_bound));
    }

    SECTION("central rotation adds 0") {
      offset_knob = 0.5f;
      module.step();
      REQUIRE_THAT(y, near(x + 0.f));
    }
  }

  SECTION("scale") {
    SECTION("max rotation yields 2x") {
      x = 2.181f;
      scale_knob = 1.f;
      module.step();
      REQUIRE_THAT(y, near(x * 2.f));
    }

    SECTION("three-quarter rotation yields x") {
      x = 3.345;
      scale_knob = 0.75f;
      module.step();
      REQUIRE_THAT(y, near(x));
    }

    SECTION("central rotation yields 0") {
      x = 2.847;
      scale_knob = 0.5f;
      module.step();
      REQUIRE_THAT(y, near(0.f));
    }

    SECTION("quarter rotation yields -x") {
      x = 4.394;
      scale_knob = 0.25f;
      module.step();
      REQUIRE_THAT(y, near(-x));
    }

    SECTION("min rotation yields -2x") {
      x = 4.394;
      scale_knob = 0.f;
      module.step();
      REQUIRE_THAT(y, near(x * -2.f));
    }
  }

  SECTION("squared") {
    scale_knob = 0.5f; // no x^1 term

    SECTION("max rotation yields 2(x^2)") {
      x = 4.872f;
      auto bipolar_squared_x = pow(x / 5.f, 2.f) * 5.f;
      squared_knob = 1.f;
      module.step();
      REQUIRE_THAT(y, near(bipolar_squared_x * 2.f));
    }

    SECTION("three-quarter rotation yields x^2") {
      x = 3.872f;
      auto bipolar_squared_x = pow(x / 5.f, 2.f) * 5.f;
      squared_knob = 0.75f;
      module.step();
      REQUIRE_THAT(y, near(bipolar_squared_x));
    }

    SECTION("central rotation yields 0") {
      x = 3.872f;
      squared_knob = 0.5f;
      module.step();
      REQUIRE_THAT(y, near(0.f));
    }

    SECTION("one-quarter rotation yields -(x^2)") {
      x = 1.367f;
      auto bipolar_squared_x = pow(x / 5.f, 2.f) * 5.f;
      squared_knob = 0.25f;
      module.step();
      REQUIRE_THAT(y, near(-bipolar_squared_x));
    }

    SECTION("min rotation yields -2(x^2)") {
      x = 4.872f;
      auto bipolar_squared_x = pow(x / 5.f, 2.f) * 5.f;
      squared_knob = 0.f;
      module.step();
      REQUIRE_THAT(y, near(bipolar_squared_x * -2.f));
    }
  }
}