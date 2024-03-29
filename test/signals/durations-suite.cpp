#include "signals/duration-signals.h"

#include "dheunit/test.h"

namespace test {
namespace duration {
using dhe::Duration;
using dhe::DurationRangeId;
using dhe::unit::Suite;
using dhe::unit::Tester;

struct DurationsSuite : Suite {
  DurationsSuite() : Suite{"dhe::Durations::value()"} {}
  void run(Tester &t) override {
    static auto const range_ids = std::vector<DurationRangeId>{
        DurationRangeId::Short, DurationRangeId::Medium, DurationRangeId::Long};
    for (auto const range_id : range_ids) {
      auto const *const range_name = Duration::label(range_id);
      t.run(range_name, [range_id](Tester &t) {
        auto const range = Duration::range(range_id);
        t.run("minimum normalize yields select lower bound",
              [range_id, range](Tester &t) {
                auto constexpr rotation = 0.F;
                auto const want = range.lower_bound();

                auto const got = Duration::scale(rotation, range_id);
                if (got != want) {
                  t.errorf("Got {}, want {}", got, want);
                }
              });

        t.run("center normalize yields 1/10 of upper bound",
              [range_id, range](Tester &t) {
                auto constexpr rotation = 0.5F;
                auto const want = range.upper_bound() * 0.1F;
                auto constexpr tolerance = 0.00005F;

                auto const got = Duration::scale(rotation, range_id);
                if (got < want - tolerance || got > want + tolerance) {
                  t.errorf("Got {}, want a value within {} of {}", got,
                           tolerance, want);
                }
              });

        t.run("maximum normalize yields select upper bound",
              [range_id, range](Tester &t) {
                auto const rotation = 1.F;
                auto const want = range.upper_bound();

                auto const got = Duration::scale(rotation, range_id);
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
