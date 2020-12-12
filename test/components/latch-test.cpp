#include "components/latch.h"
#include "helpers/latches.h"

#include <dheunit/test.h>

#include <functional>

using dhe::Latch;
using dhe::unit::Suite;
using dhe::unit::Tester;

namespace dhe {
namespace components {
namespace test {

using TestFunc = std::function<void(Tester &)>;

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

  void run(Tester &t) override {
    t.run("is low by default", check_equality(Latch{}, low, true));

    t.run("high == high", check_equality(high, high, true));
    t.run("high != low", check_equality(high, low, false));
    t.run("high != rising", check_equality(high, rising, false));
    t.run("high != falling", check_equality(high, falling, false));
    t.run("low != high", check_equality(low, high, false));
    t.run("low == low", check_equality(low, low, true));
    t.run("low != rising", check_equality(low, rising, false));
    t.run("low != falling", check_equality(low, falling, false));
    t.run("rising != high", check_equality(rising, high, false));
    t.run("rising != low", check_equality(rising, low, false));
    t.run("rising == rising", check_equality(rising, rising, true));
    t.run("rising != falling", check_equality(rising, falling, false));
    t.run("falling != high", check_equality(falling, high, false));
    t.run("falling != low", check_equality(falling, low, false));
    t.run("falling != rising", check_equality(falling, rising, false));
    t.run("falling == falling", check_equality(falling, falling, true));

    t.run("high + high signal → unchanged", check_clock(high, true, high));
    t.run("high + low signal → falls", check_clock(high, false, falling));

    t.run("low + high signal → rises", check_clock(low, true, rising));
    t.run("low + low signal → unchanged", check_clock(low, false, low));

    t.run("rising + high signal → loses edge", check_clock(rising, true, high));
    t.run("rising + low signal → falling", check_clock(rising, false, falling));

    t.run("falling + high signal → rises", check_clock(falling, true, rising));
    t.run("falling + low signal → loses edge",
          check_clock(falling, false, low));
  }
};

__attribute__((unused)) static auto _ = LatchSuite{};

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

} // namespace test
} // namespace components
} // namespace dhe
