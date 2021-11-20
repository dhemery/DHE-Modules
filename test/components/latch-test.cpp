#include "components/latch.h"
#include "helpers/latches.h"

#include "dheunit/test.h"

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

static auto require_equal(const Latch &a, const Latch &b) -> TestFunc;
static auto require_unequal(const Latch &a, const Latch &b) -> TestFunc;
static auto check_clock(Latch l, bool signal, Latch want_latch) -> TestFunc;

class LatchSuite : public Suite {
public:
  LatchSuite() : Suite{"dhe::Latch"} {}

  void run(Tester &t) override {
    t.run("is low by default", require_equal(Latch{}, low));

    t.run("high == high", require_equal(high, high));
    t.run("high != low", require_unequal(high, low));
    t.run("high != rising", require_unequal(high, rising));
    t.run("high != falling", require_unequal(high, falling));
    t.run("low != high", require_unequal(low, high));
    t.run("low == low", require_equal(low, low));
    t.run("low != rising", require_unequal(low, rising));
    t.run("low != falling", require_unequal(low, falling));
    t.run("rising != high", require_unequal(rising, high));
    t.run("rising != low", require_unequal(rising, low));
    t.run("rising == rising", require_equal(rising, rising));
    t.run("rising != falling", require_unequal(rising, falling));
    t.run("falling != high", require_unequal(falling, high));
    t.run("falling != low", require_unequal(falling, low));
    t.run("falling != rising", require_unequal(falling, rising));
    t.run("falling == falling", require_equal(falling, falling));

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

auto require_unequal(const Latch &a, const Latch &b) -> TestFunc {
  return [a, b](Tester &t) {
    if (a == b) {
      t.errorf("{} == {} want false, got true", a, b);
    }
    if (b == a) {
      t.errorf("{} == {} want false, got true", b, a);
    }
    if (!(a != b)) {
      t.errorf("{} != {} want true, got false", a, b);
    }
    if (!(b != a)) {
      t.errorf("{} != {} want true, got false", b, a);
    }
  };
}

auto require_equal(const Latch &a, const Latch &b) -> TestFunc {
  return [a, b](Tester &t) {
    if (!(a == b)) {
      t.errorf("{}=={} want true, got false", a, b);
    }
    if (!(b == a)) {
      t.errorf("{}=={} want true, got false", b, a);
    }
    if (a != b) {
      t.errorf("{}!={} want false, got true", a, b);
    }
    if (b != a) {
      t.errorf("{}!={} want false, got true", b, a);
    }
  };
}

} // namespace test
} // namespace components
} // namespace dhe
