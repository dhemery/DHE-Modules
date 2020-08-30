#include "components/range.h"
#include "controls/duration-inputs.h"
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

// The proper form for a duration range: max = 1000 * min
static auto const range = Range{0.001F, 1.F};

class DurationInputSuite : public Suite {
public:
  DurationInputSuite() : Suite{"dhe::duration(k,r)"} {}
  void register_tests(TestRegistrar add) override {
    add("minimum rotation yields range lower bound", [](Tester &t) {
      auto constexpr knob = Knob{0.F};
      t.assert_that(dhe::duration(knob, range),
                    is_equal_to(range.lower_bound()));
    });

    add("center rotation yields 1/10 of upper bound", [](Tester &t) {
      auto constexpr knob = Knob{0.5F};

      t.assert_that(dhe::duration(knob, range),
                    is_near(range.upper_bound() * 0.1F, 0.0000001F));
    });

    add("maximum rotation yields range upper bound", [](Tester &t) {
      auto knob = Knob{1.F};
      t.assert_that(dhe::duration(knob, range),
                    is_equal_to(range.upper_bound()));
    });
  }
};

__attribute__((unused)) static auto _ = DurationInputSuite{};
} // namespace duration
} // namespace test