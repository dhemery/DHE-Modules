#include "./fixtures/duration-fixture.h"

#include <dheunit/test.h>

namespace test {
namespace sequencizer {
using dhe::unit::is_equal_to;
using dhe::unit::is_near;
using dhe::unit::Suite;
using dhe::unit::TestRegistrar;

auto constexpr short_duration_selection = 0;
auto constexpr medium_duration_selection = 1;
auto constexpr long_duration_selection = 2;

auto constexpr minimum_duration_rotation = 0.F;
auto constexpr center_duration_rotation = 0.5F;
auto constexpr maximum_duration_rotation = 1.F;

auto constexpr minimum_multiplier_rotation = 0.F;
auto constexpr center_multiplier_rotation = 0.5F;
auto constexpr maximum_multiplier_rotation = 1.F;

class DurationSuite : public Suite {
public:
  DurationSuite() : Suite("dhe::sequencizer::duration()") {}
  void register_tests(TestRegistrar add) override {
    add("with 0V multiplier cv: "
        "center multiplier rotation: "
        "yields nominal duration",
        test(0.731F, short_duration_selection, center_multiplier_rotation, 0.F,
             is_equal_to(dhe::duration(0.731F, dhe::short_duration_range))));

    add("with 0V multiplier cv: "
        "maximum multiplier rotation: "
        "yields twice nominal duration",
        test(0.4623F, medium_duration_selection, maximum_multiplier_rotation,
             0.F,
             is_equal_to(2.F *
                         dhe::duration(0.4623F, dhe::medium_duration_range))));

    add("5V CV adds 50% rotation to multiplier knob",
        // 25% multiplier knob + 50% CV = 75% rotation = 1.5 multiplier
        test(center_duration_rotation, long_duration_selection, 0.25F, 5.F,
             is_equal_to(1.5F * dhe::duration(center_duration_rotation,
                                              dhe::long_duration_range))));

    add("-5V CV subtracts 50% rotation from multiplier knob",
        // 90% multiplier knob - 50% CV = 40% rotation = 0.8 multiplier
        test(center_duration_rotation, short_duration_selection, 0.90F, -5.F,
             is_near(0.8F * dhe::duration(center_duration_rotation,
                                          dhe::short_duration_range),
                     1e-5F)));

    add("minimum short duration is 1ms",
        test(minimum_duration_rotation, short_duration_selection,
             minimum_multiplier_rotation, -10.F, is_equal_to(1e-3F)));

    add("minimum medium duration is 1ms",
        test(minimum_duration_rotation, medium_duration_selection,
             minimum_multiplier_rotation, -10.F, is_equal_to(1e-3F)));

    add("minimum long duration is 1ms",
        test(maximum_duration_rotation, long_duration_selection,
             minimum_multiplier_rotation, -10.F, is_equal_to(1e-3F)));

    add("maximum short duration is twice short duration upper bound",
        test(maximum_duration_rotation, short_duration_selection,
             maximum_multiplier_rotation, 10.F,
             is_equal_to(2.F * dhe::short_duration_range.upper_bound())));

    add("maximum medium duration is twice medium duration upper bound",
        test(maximum_duration_rotation, medium_duration_selection,
             maximum_multiplier_rotation, 10.F,
             is_equal_to(2.F * dhe::medium_duration_range.upper_bound())));

    add("maximum long duration is twice long duration upper bound",
        test(maximum_duration_rotation, long_duration_selection,
             maximum_multiplier_rotation, 10.F,
             is_equal_to(2.F * dhe::long_duration_range.upper_bound())));
  }
};

static auto _ = DurationSuite{};

} // namespace sequencizer
} // namespace test
