#include "controls/level-inputs.h"

#include <doctest.h>

namespace dhe {

namespace leveltest {
class Knob {
  float rotation;

public:
  Knob(float rotation) : rotation{rotation} {};
  auto getValue() const -> float { return rotation; }
};
} // namespace leveltest

TEST_CASE("LevelInputs") {
  // tolerance = 6 decimal places
  static auto constexpr tolerance{0.000001F};

  Range range = Range{3.8423F, 9.221F};

  SUBCASE("value at minimum rotation is range lower bound") {
    leveltest::Knob knob{0.F};

    auto const level = dhe::level(knob, range);

    auto const expected = range.lower_bound();

    CHECK_EQ(level, doctest::Approx(expected).epsilon(tolerance));
  }

  SUBCASE("value at middle rotation is range midpoint") {
    leveltest::Knob knob{0.5F};

    auto const level = dhe::level(knob, range);

    auto const expected = range.size() * 0.5 + range.lower_bound();
    CHECK_EQ(level, doctest::Approx(expected).epsilon(tolerance));
  }

  SUBCASE("value at maximum rotation is range upper bound") {
    leveltest::Knob knob{1.F};

    auto const level = dhe::level(knob, range);

    auto const expected = range.upper_bound();
    CHECK_EQ(level, doctest::Approx(expected).epsilon(tolerance));
  }
}
} // namespace dhe
