#include "signals/duration-ranges.h"

#include "components/range.h"

#include "dheunit/test.h"

namespace test {
namespace duration {
using dhe::Range;
using dhe::unit::Suite;
using dhe::unit::Tester;

class DurationsSuite : public Suite {
public:
  DurationsSuite() : Suite{"dhe::DurationRanges::value()"} {}
  void run(Tester &t) override {
    for (int selection = dhe::DurationRanges::Short;
         selection <= dhe::DurationRanges::Long; selection++) {
      auto const range = dhe::DurationRanges::items()[selection];
      t.run(dhe::DurationRanges::labels()[selection], [selection,
                                                       range](Tester &t) {
        t.run("minimum rotation yields range lower bound", [selection,
                                                            range](Tester &t) {
          auto constexpr rotation = 0.F;
          auto const want = range.lower_bound();

          auto const got = dhe::DurationRanges::value(rotation, selection);
          if (got != want) {
            t.errorf("Got {}, want {}", got, want);
          }
        });

        t.run("center rotation yields 1/10 of upper bound", [selection,
                                                             range](Tester &t) {
          auto constexpr rotation = 0.5F;
          auto const want = range.upper_bound() * 0.1F;
          auto constexpr tolerance = 0.00005F;

          auto const got = dhe::DurationRanges::value(rotation, selection);
          if (got < want - tolerance || got > want + tolerance) {
            t.errorf("Got {}, want a value within {} of {}", got, tolerance,
                     want);
          }
        });

        t.run("maximum rotation yields range upper bound", [selection,
                                                            range](Tester &t) {
          auto const rotation = 1.F;
          auto const want = range.upper_bound();

          auto const got = dhe::DurationRanges::value(rotation, selection);
          if (got != want) {
            t.errorf("Got {}, want {}", got, want);
          }
        });
      });
    }
  }
};

static auto _ = DurationsSuite{};
} // namespace duration
} // namespace test
