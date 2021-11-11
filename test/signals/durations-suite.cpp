#include "signals/durations.h"

#include "components/range.h"

#include "dheunit/test.h"

namespace test {
namespace duration {
using dhe::Durations;
using dhe::Range;
using dhe::unit::Suite;
using dhe::unit::Tester;

class DurationsSuite : public Suite {
public:
  DurationsSuite() : Suite{"dhe::Durations::value()"} {}
  void run(Tester &t) override {
    for (int selection = Durations::Short; selection <= Durations::Long;
         selection++) {
      auto const range = Durations::select(selection);
      t.run(Durations::labels()[selection], [selection, range](Tester &t) {
        t.run("minimum rotation yields range lower bound",
              [selection, range](Tester &t) {
                auto constexpr rotation = 0.F;
                auto const want = range.lower_bound();

                auto const got = Durations::seconds(rotation, selection);
                if (got != want) {
                  t.errorf("Got {}, want {}", got, want);
                }
              });

        t.run("center rotation yields 1/10 of upper bound",
              [selection, range](Tester &t) {
                auto constexpr rotation = 0.5F;
                auto const want = range.upper_bound() * 0.1F;
                auto constexpr tolerance = 0.00005F;

                auto const got = Durations::seconds(rotation, selection);
                if (got < want - tolerance || got > want + tolerance) {
                  t.errorf("Got {}, want a value within {} of {}", got,
                           tolerance, want);
                }
              });

        t.run("maximum rotation yields range upper bound",
              [selection, range](Tester &t) {
                auto const rotation = 1.F;
                auto const want = range.upper_bound();

                auto const got = Durations::seconds(rotation, selection);
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
