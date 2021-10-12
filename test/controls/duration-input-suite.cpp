#include "components/range.h"
#include "controls/duration-inputs.h"
#include "helpers/rack-controls.h"

#include <dheunit/test.h>

namespace test {
namespace duration {
using dhe::Range;
using dhe::unit::Suite;
using dhe::unit::Tester;
using Knob = fake::Param;

// The proper boundaries for a duration range: upper = 1000 * lower
static auto constexpr range = Range{0.001F, 1.F};

class DurationInputSuite : public Suite {
public:
  DurationInputSuite() : Suite{"dhe::duration(k,r)"} {}
  void run(Tester &t) override {
    t.run("minimum rotation yields range lower bound", [](Tester &t) {
      auto constexpr knob = Knob{0.F};
      auto constexpr want = range.lower_bound();

      auto const got = dhe::duration(knob, range);
      if (got != want) {
        t.errorf("Got {}, want {}", got, want);
      }
    });

    t.run("center rotation yields 1/10 of upper bound", [](Tester &t) {
      auto constexpr knob = Knob{0.5F};
      auto constexpr want = range.upper_bound() * 0.1F;
      auto constexpr tolerance = 0.0000001F;

      auto const got = dhe::duration(knob, range);
      if (got < want - tolerance || got > want + tolerance) {
        t.errorf("Got {}, want a value within {} of {}", got, tolerance, want);
      }
    });

    t.run("maximum rotation yields range upper bound", [](Tester &t) {
      auto knob = Knob{1.F};
      auto constexpr want = range.upper_bound();

      auto const got = dhe::duration(knob, range);
      if (got != want) {
        t.errorf("Got {}, want {}", got, want);
      }
    });
  }
};

static auto _ = DurationInputSuite{};
} // namespace duration
} // namespace test
