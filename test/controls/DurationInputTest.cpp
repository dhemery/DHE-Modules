#include "controls/DurationInputs.h"

#include "doctest/doctest.h"

namespace dhe {

namespace durationtest {
  class Knob {
    float rotation;

  public:
    Knob(float rotation) : rotation{rotation} {};
    auto getValue() const -> float { return rotation; }
  };


} // namespace durationtest

TEST_CASE("DurationInputs") {
  Range range = Range{2.1874F, 11.09234F};

  SUBCASE("value at minimum rotation is range lower bound") {
    durationtest::Knob knob{0.F};

    auto const level = dhe::duration(knob, range);

    auto const expected = range.lowerBound();

    CHECK_EQ(level, doctest::Approx(expected).epsilon(0.000001F));
  }

  SUBCASE("value at middle rotation is near one tenth of the way into the range") {
    durationtest::Knob knob{0.5F};

    auto const level = dhe::duration(knob, range);

    auto const expected = (range.upperBound() - range.lowerBound()) * 0.1F + range.lowerBound();
    CHECK_EQ(level, doctest::Approx(expected).epsilon(0.005F));
  }

  SUBCASE("value at maximum rotation is range upper bound") {
    durationtest::Knob knob{1.F};

    auto const level = dhe::duration(knob, range);

    auto const expected = range.upperBound();
    CHECK_EQ(level, doctest::Approx(expected).epsilon(0.000001F));
  }
}
} // namespace dhe
