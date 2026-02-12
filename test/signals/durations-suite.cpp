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
    static auto const duration_curves = std::vector<DurationCurve>{
        dhe::short_duration_curve,
        dhe::medium_duration_curve,
        dhe::long_duration_curve,
    };
    for (auto const &duration_curve : duration_curves) {
      const auto &range_name = duration_curve.label();
      t.run(range_name, [duration_curve](Tester &t) {
        t.run("minimum normalize yields lower bound",
              [duration_curve](Tester &t) {
                auto constexpr rotation = 0.F;
                auto const want = duration_curve.range().lower_bound();

                auto const got = duration_curve.scale(rotation);
                if (got != want) {
                  t.errorf("Got {}, want {}", got, want);
                }
              });

        t.run("center normalize yields 1/10 of upper bound",
              [duration_curve](Tester &t) {
                auto constexpr rotation = 0.5F;
                auto const want = duration_curve.range().upper_bound() * 0.1F;
                auto constexpr tolerance = 0.00005F;

                auto const got = duration_curve.scale(rotation);
                if (got < want - tolerance || got > want + tolerance) {
                  t.errorf("Got {}, want a value within {} of {}", got,
                           tolerance, want);
                }
              });

        t.run("maximum normalize yields upper bound",
              [duration_curve](Tester &t) {
                auto const rotation = 1.F;
                auto const want = duration_curve.range().upper_bound();

                auto const got = duration_curve.scale(rotation);
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
