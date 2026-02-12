#include "signals/duration.h"

#include "dheunit/test.h"

namespace test {
namespace duration {
using dhe::DurationCurve;
using dhe::unit::Suite;
using dhe::unit::Tester;

struct DurationsSuite : Suite {
  DurationsSuite() : Suite{"dhe::Durations::value()"} {}
  void run(Tester &t) override {
    static auto const range_ids = std::vector<DurationCurve::Id>{
        DurationCurve::Id::Short, DurationCurve::Id::Medium,
        DurationCurve::Id::Long};
    for (auto const range_id : range_ids) {
      auto range_name = DurationCurve::label(range_id);
      t.run(range_name, [range_id](Tester &t) {
        auto const taper = DurationCurve::by_id(range_id);
        t.run("minimum normalize yields lower bound", [taper](Tester &t) {
          auto constexpr rotation = 0.F;
          auto const want = taper.range().lower_bound();

          auto const got = taper.scale(rotation);
          if (got != want) {
            t.errorf("Got {}, want {}", got, want);
          }
        });

        t.run("center normalize yields 1/10 of upper bound",
              [taper](Tester &t) {
                auto constexpr rotation = 0.5F;
                auto const want = taper.range().upper_bound() * 0.1F;
                auto constexpr tolerance = 0.00005F;

                auto const got = taper.scale(rotation);
                if (got < want - tolerance || got > want + tolerance) {
                  t.errorf("Got {}, want a value within {} of {}", got,
                           tolerance, want);
                }
              });

        t.run("maximum normalize yields upper bound", [taper](Tester &t) {
          auto const rotation = 1.F;
          auto const want = taper.range().upper_bound();

          auto const got = taper.scale(rotation);
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
