#include "helpers/rack-controls.h"
#include "modules/sequencizer/module.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace sequencizer {

static auto constexpr step_count = 8;

using dhe::unit::Tester;
using dhe::unit::TestFunc;
using test::fake::Param;
using test::fake::Port;

template <typename Matcher>
static inline auto test(float duration_rotation, int range_selection,
                        float multiplier_rotation, float multiplier_cv_voltage,
                        Matcher matcher) -> TestFunc {
  return [duration_rotation, range_selection, multiplier_rotation,
          multiplier_cv_voltage, matcher](Tester &t) {
    Param duration_knob{duration_rotation};
    Param range_switch{static_cast<float>(range_selection)};
    Param multiplier_knob{multiplier_rotation};
    Port multiplier_cv_input{multiplier_cv_voltage};

    auto const actual = dhe::sequencizer::duration(
        duration_knob, range_switch, multiplier_knob, multiplier_cv_input);

    t.assert_that(actual, matcher);
  };
}

} // namespace sequencizer
} // namespace test
