#pragma once

#include "dheunit/test.h"

#include <functional>
#include <string>

namespace test {

using dhe::unit::Tester;

template <typename Subject, typename Assertion>
void assert_that(Tester &t, std::string const &context, Subject const &subject,
                 Assertion const &assertion) {
  t.run(context, [&subject, &assertion](Tester &t) { assertion(t, subject); });
}

template <typename Subject, typename Assertion>
void assert_that(Tester &t, Subject const &actual, Assertion const &assertion) {
  assertion(t, actual);
}

template <typename Actual>
auto is_equal_to(Actual const &want)
    -> std::function<void(Tester &, Actual const &)> {
  return [want](Tester &t, Actual const &actual) {
    if (actual != want) {
      t.errorf("was {}, want {}", actual, want);
    }
  };
}

template <typename Actual>
auto is_near(Actual const &want, Actual const &tolerance)
    -> std::function<void(Tester &, Actual const &)> {
  return [want, tolerance](Tester &t, Actual const &actual) {
    auto const min = want - tolerance;
    auto const max = want + tolerance;
    if (actual < min || actual > max) {
      t.errorf("was {}, want within {} of {}", actual, tolerance, want);
    }
  };
}

template <typename Actual>
auto is_less_than(Actual const &min)
    -> std::function<void(Tester &, Actual const &)> {
  return [min](Tester &t, Actual const &actual) {
    if (!(actual < min)) {
      t.errorf("was {}, want less than {}", actual, min);
    }
  };
}

static inline void is_true(Tester &t, bool actual) {
  if (!actual) {
    t.error("was false, want true");
  }
}

static inline void is_false(Tester &t, bool actual) {
  if (actual) {
    t.error("was true, want false");
  }
}
} // namespace test
