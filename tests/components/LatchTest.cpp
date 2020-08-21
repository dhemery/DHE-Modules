#include "Latch.h"

#include "dheunit/test.h"

using dhe::Latch;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;
using dhe::unit::TestRegistrar;

namespace dhe {
namespace components {
static auto constexpr low = Latch{false, false};
static auto constexpr falling = Latch{false, true};
static auto constexpr rising = Latch{true, true};
static auto constexpr high = Latch{true, false};

static auto check_equality(const Latch &a, const Latch &b, bool want_eq)
    -> TestFunc;
static auto check_clock(Latch l, bool signal, Latch want_latch) -> TestFunc;

class LatchSuite : public Suite {
public:
  LatchSuite() : Suite{"dhe::Latch"} {}

  void register_tests(TestRegistrar add_test) override {
    add_test("is low by default", check_equality(Latch{}, low, true));

    add_test("high == high", check_equality(high, high, true));
    add_test("high != low", check_equality(high, low, false));
    add_test("high != rising", check_equality(high, rising, false));
    add_test("high != falling", check_equality(high, falling, false));
    add_test("low != high", check_equality(low, high, false));
    add_test("low == low", check_equality(low, low, true));
    add_test("low != rising", check_equality(low, rising, false));
    add_test("low != falling", check_equality(low, falling, false));
    add_test("rising != high", check_equality(rising, high, false));
    add_test("rising != low", check_equality(rising, low, false));
    add_test("rising == rising", check_equality(rising, rising, true));
    add_test("rising != falling", check_equality(rising, falling, false));
    add_test("falling != high", check_equality(falling, high, false));
    add_test("falling != low", check_equality(falling, low, false));
    add_test("falling != rising", check_equality(falling, rising, false));
    add_test("falling == falling", check_equality(falling, falling, true));

    add_test("high + high signal → unchanged", check_clock(high, true, high));
    add_test("high + low signal → falls", check_clock(high, false, falling));

    add_test("low + high signal → rises", check_clock(low, true, rising));
    add_test("low + low signal → unchanged", check_clock(low, false, low));

    add_test("rising + high signal → loses edge",
             check_clock(rising, true, high));
    add_test("rising + low signal → falling",
             check_clock(rising, false, falling));

    add_test("falling + high signal → rises",
             check_clock(falling, true, rising));
    add_test("falling + low signal → loses edge",
             check_clock(falling, false, low));
  }
};

static LatchSuite latch_suite __attribute__((unused)){};

auto check_clock(Latch l, bool signal, Latch want_latch) -> TestFunc {
  return [l, signal, want_latch](Tester &t) mutable {
    l.clock(signal);

    if (l != want_latch) {
      t.errorf("Got {}, want {}", l, want_latch);
    }
  };
}

auto check_equality(const Latch &a, const Latch &b, bool want_eq) -> TestFunc {
  return [a, b, want_eq](Tester &t) {
    auto const aeqb = (a == b);
    if (aeqb != want_eq) {
      t.errorf("{}=={} got {}, want {}", a, b, aeqb, !aeqb);
    }
    auto const beqa = (b == a);
    if (beqa != want_eq) {
      t.errorf("{}=={} got {}, want {}", b, a, beqa, !beqa);
    }
    auto const aneb = (a != b);
    if (aneb == want_eq) {
      t.errorf("{}!={} got {}, want {}", a, b, aneb, !aneb);
    }
    auto const bnea = (b != a);
    if (bnea == want_eq) {
      t.errorf("{}!={} got {}, want {}", b, a, bnea, !bnea);
    }
  };
}

} // namespace components
} // namespace dhe
