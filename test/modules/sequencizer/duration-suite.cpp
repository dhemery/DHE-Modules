#include "fixtures/duration-fixture.h"

#include "dheunit/test.h"
#include "signals/durations.h"

namespace test {
namespace sequencizer {
using dhe::unit::Suite;

auto constexpr short_duration_selection = 0;
auto constexpr medium_duration_selection = 1;
auto constexpr long_duration_selection = 2;

auto constexpr minimum_duration_rotation = 0.F;
auto constexpr center_duration_rotation = 0.5F;
auto constexpr maximum_duration_rotation = 1.F;

auto constexpr minimum_multiplier_rotation = 0.F;
auto constexpr center_multiplier_rotation = 0.5F;
auto constexpr maximum_multiplier_rotation = 1.F;

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
          test(0.731F, short_duration_selection, center_multiplier_rotation,
               0.F,
               is_equal_to(dhe::duration::scale(
                   0.731F, dhe::duration_ranges::short_range))));

    t.run("with 0V multiplier cv: "
          "maximum multiplier rotation: "
          "yields twice nominal duration",
          test(0.4623F, medium_duration_selection, maximum_multiplier_rotation,
               0.F,
               is_near(2.F * dhe::duration::scale(
                                 0.4623F, dhe::duration_ranges::medium_range),
                       1e-5F)));

    t.run("5V CV adds 50% rotation to multiplier knob",
          // 25% multiplier knob + 50% CV = 75% rotation = 1.5 multiplier
          test(center_duration_rotation, long_duration_selection, 0.25F, 5.F,
               is_equal_to(1.5F * dhe::duration::scale(
                                      center_duration_rotation,
                                      dhe::duration_ranges::long_range))));

    t.run("-5V CV subtracts 50% rotation from multiplier knob",
          // 90% multiplier knob - 50% CV = 40% rotation = 0.8 multiplier
          test(center_duration_rotation, short_duration_selection, 0.90F, -5.F,
               is_near(0.8F * dhe::duration::scale(
                                  center_duration_rotation,
                                  dhe::duration_ranges::short_range),
                       1e-5F)));

    t.run("minimum short duration is 1ms",
          test(minimum_duration_rotation, short_duration_selection,
               minimum_multiplier_rotation, -10.F, is_equal_to(1e-3F)));

    t.run("minimum medium duration is 1ms",
          test(minimum_duration_rotation, medium_duration_selection,
               minimum_multiplier_rotation, -10.F, is_equal_to(1e-3F)));

    t.run("minimum long duration is 1ms",
          test(maximum_duration_rotation, long_duration_selection,
               minimum_multiplier_rotation, -10.F, is_equal_to(1e-3F)));

    t.run("maximum short duration is twice short duration upper bound",
          test(maximum_duration_rotation, short_duration_selection,
               maximum_multiplier_rotation, 10.F,
               is_equal_to(2.F *
                           dhe::duration_ranges::short_range.upper_bound())));

    t.run("maximum medium duration is twice medium duration upper bound",
          test(maximum_duration_rotation, medium_duration_selection,
               maximum_multiplier_rotation, 10.F,
               is_equal_to(2.F *
                           dhe::duration_ranges::medium_range.upper_bound())));

    t.run("maximum long duration is twice long duration upper bound",
          test(maximum_duration_rotation, long_duration_selection,
               maximum_multiplier_rotation, 10.F,
               is_equal_to(2.F *
                           dhe::duration_ranges::long_range.upper_bound())));
  }
};

static auto _ = DurationSuite{};
} // namespace sequencizer
} // namespace test
