#include <lib/catch.hpp>
#include <modules/cubic-module.h>
#include <lib/matchers.h>

TEST_CASE("Cubic Module") {
  DHE::CubicModule module{};
  auto &offset_knob = module.params[module.CONSTANT].value;
  auto &out = module.outputs[module.OUT].value;

  SECTION("offset") {
    SECTION("max rotation yields max bipolar signal") {
      offset_knob = 1.f;
      module.step();
      REQUIRE_THAT(out, near(DHE::BIPOLAR_SIGNAL_RANGE.upper_bound));
    }

    SECTION("min rotation yields max bipolar signal") {
      offset_knob = 0.f;
      module.step();
      REQUIRE_THAT(out, near(DHE::BIPOLAR_SIGNAL_RANGE.lower_bound));
    }

    SECTION("central rotation yields central bipolar signal") {
      offset_knob = 0.5f;
      module.step();
      REQUIRE_THAT(out, near(DHE::BIPOLAR_SIGNAL_RANGE.scale(0.5f)));
    }
  }
}