#include "components/range.h"
#include "controls/level-inputs.h"
#include "helpers/rack-controls.h"

#include "dheunit/test.h"

namespace test {
namespace duration {
using dhe::Range;
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
      auto const want = range.lower_bound();
      auto const got = dhe::level(knob, range);

      if (got != want) {
        t.errorf("Got {}, want {}", got, want);
      }
    });

    t.run("center rotation yields range midpoint", [](Tester &t) {
      auto constexpr knob = Knob{0.5F};
      auto constexpr midpoint = range.scale(0.5F);
      auto constexpr tolerance = 0.0000001F;

      auto const got = dhe::level(knob, range);
      if (got < midpoint - tolerance || got > midpoint + tolerance) {
        t.errorf("Got {}, want a value within {} of {}", got, tolerance,
                 midpoint);
      }
    });

    t.run("maximum rotation yields range upper bound", [](Tester &t) {
      auto constexpr knob = Knob{1.F};
      auto constexpr want = range.upper_bound();

      auto const got = dhe::level(knob, range);
      if (got != want) {
        t.errorf("Got {}, want {}", got, want);
      }
    });
  }
};

static auto _ = LevelInputSuite{};
} // namespace duration
} // namespace test
