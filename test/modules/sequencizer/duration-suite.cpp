#include "fixtures/duration-fixture.h"

#include "dheunit/test.h"
#include "signals/durations.h"

namespace test {
namespace sequencizer {
using dhe::Duration;
using dhe::LongDuration;
using dhe::MediumDuration;
using dhe::ShortDuration;
using dhe::unit::Suite;

static inline auto is_equal_to(float want)
    -> std::function<void(Tester &, float)> {
  return [want](Tester &t, float got) {
    if (got != want) {
      t.errorf("Got {}, want {}", got, want);
    }
  };
}

static inline auto is_near(float want, float tolerance)
    -> std::function<void(Tester &, float)> {
  return [want, tolerance](Tester &t, float got) {
    if (got < want - tolerance || got > want + tolerance) {
      t.errorf("Got {}, want a value within {} of {}", got, tolerance, want);
    }
  };
}

class DurationSuite : public Suite {
public:
  DurationSuite() : Suite("dhe::sequencizer::duration()") {}
  void run(Tester &t) override {
    t.run("with 0V multiplier cv: "
          "center multiplier rotation: "
          "yields nominal duration",
          test(0.731F, DurationRangeId::Short, 0.5F, 0.F,
               is_equal_to(ShortDuration::scale(0.731F))));

    t.run("with 0V multiplier cv: "
          "maximum multiplier rotation: "
          "yields twice nominal duration",
          test(0.4623F, DurationRangeId::Medium, 1.F, 0.F,
               is_near(2.F * MediumDuration::scale(0.4623F), 1e-5F)));

    t.run("5V CV adds 50% rotation to multiplier knob",
          // 25% multiplier knob + 50% CV = 75% rotation = 1.5 multiplier
          test(0.5F, DurationRangeId::Long, 0.25F, 5.F,
               is_equal_to(1.5F * LongDuration::scale(0.5F))));

    t.run("-5V CV subtracts 50% rotation from multiplier knob",
          // 90% multiplier knob - 50% CV = 40% rotation = 0.8 multiplier
          test(0.5F, DurationRangeId::Short, 0.90F, -5.F,
               is_near(0.8F * ShortDuration::scale(0.5F), 1e-5F)));

    t.run("minimum short duration is 1ms",
          test(0.F, DurationRangeId::Short, 0.F, -30.F, is_equal_to(1e-3F)));

    t.run("minimum medium duration is 1ms",
          test(0.F, DurationRangeId::Medium, 0.F, -30.F, is_equal_to(1e-3F)));

    t.run("minimum long duration is 1ms",
          test(0.F, DurationRangeId::Long, 0.F, -30.F, is_equal_to(1e-3F)));

    t.run("maximum short duration is 2s",
          test(1.F, DurationRangeId::Short, 1.F, 30.F, is_equal_to(2.F)));

    t.run("maximum medium duration is 20s",
          test(1.F, DurationRangeId::Medium, 1.F, 30.F, is_equal_to(20.F)));

    t.run("maximum long duration is 200s",
          test(1.F, DurationRangeId::Long, 1.F, 30.F, is_equal_to(200.F)));
  }
};

static auto _ = DurationSuite{};
} // namespace sequencizer
} // namespace test
