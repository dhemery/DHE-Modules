#include "components/range.h"
#include "controls/duration-inputs.h"
#include "helpers/rack-controls.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace duration {
using dhe::Range;
using dhe::unit::is_equal_to;
using dhe::unit::is_near;
using dhe::unit::Suite;
using dhe::unit::Tester;
using Knob = fake::Param;

// The proper form for a duration range: max = 1000 * min
static auto const range = Range{0.001F, 1.F};

class DurationInputSuite : public Suite {
public:
  DurationInputSuite() : Suite{"dhe::duration(k,r)"} {}
  void run(Tester &t) override {
    t.run("minimum rotation yields range lower bound", [](Tester &t) {
      auto constexpr knob = Knob{0.F};
      t.assert_that(dhe::duration(knob, range),
                    is_equal_to(range.lower_bound()));
    });

    t.run("center rotation yields 1/10 of upper bound", [](Tester &t) {
      auto constexpr knob = Knob{0.5F};

      t.assert_that(dhe::duration(knob, range),
                    is_near(range.upper_bound() * 0.1F, 0.0000001F));
    });

    t.run("maximum rotation yields range upper bound", [](Tester &t) {
      auto knob = Knob{1.F};
      t.assert_that(dhe::duration(knob, range),
                    is_equal_to(range.upper_bound()));
    });
  }
};

__attribute__((unused)) static auto _ = DurationInputSuite{};
} // namespace duration
} // namespace test