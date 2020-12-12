#include "components/range.h"
#include "controls/level-inputs.h"
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

static auto constexpr range = Range{3.8423F, 9.221F};

class LevelInputSuite : public Suite {
public:
  LevelInputSuite() : Suite{"dhe::level(k,r)"} {}
  void run(Tester &t) override {
    t.run("minimum rotation yields range lower bound", [](Tester &t) {
      auto constexpr knob = Knob{0.F};
      t.assert_that(dhe::level(knob, range), is_equal_to(range.lower_bound()));
    });

    t.run("center rotation yields range midpoint", [](Tester &t) {
      auto constexpr knob = Knob{0.5F};

      auto constexpr midpoint =
          range.lower_bound() +
          0.5F * (range.upper_bound() - range.lower_bound());
      t.assert_that(dhe::level(knob, range), is_near(midpoint, 0.0000001F));
    });

    t.run("maximum rotation yields range upper bound", [](Tester &t) {
      auto knob = Knob{1.F};
      t.assert_that(dhe::level(knob, range), is_equal_to(range.upper_bound()));
    });
  }
};

__attribute__((unused)) static auto _ = LevelInputSuite{};
} // namespace duration
} // namespace test