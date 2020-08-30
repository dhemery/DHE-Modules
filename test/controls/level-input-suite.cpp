#include "components/range.h"
#include "controls/level-inputs.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace duration {
using dhe::Range;
using dhe::unit::is_equal_to;
using dhe::unit::is_near;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestRegistrar;

struct Knob {
  constexpr Knob(float rotation) : rotation_{rotation} {}
  constexpr auto getValue() const -> float { return rotation_; } // NOLINT
  float rotation_{-99.F};                                        // NOLINT
};

static auto constexpr range = Range{3.8423F, 9.221F};

class LevelInputSuite : public Suite {
public:
  LevelInputSuite() : Suite{"dhe::level(k,r)"} {}
  void register_tests(TestRegistrar add) override {
    add("minimum rotation yields range lower bound", [](Tester &t) {
      auto constexpr knob = Knob{0.F};
      t.assert_that(dhe::level(knob, range), is_equal_to(range.lower_bound()));
    });

    add("center rotation yields range midpoint", [](Tester &t) {
      auto constexpr knob = Knob{0.5F};

      auto constexpr midpoint =
          range.lower_bound() +
          0.5F * (range.upper_bound() - range.lower_bound());
      t.assert_that(dhe::level(knob, range), is_near(midpoint, 0.0000001F));
    });

    add("maximum rotation yields range upper bound", [](Tester &t) {
      auto knob = Knob{1.F};
      t.assert_that(dhe::level(knob, range), is_equal_to(range.upper_bound()));
    });
  }
};

__attribute__((unused)) static auto _ = LevelInputSuite{};
} // namespace duration
} // namespace test