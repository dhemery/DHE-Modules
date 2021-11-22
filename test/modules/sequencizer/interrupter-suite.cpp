#include "modules/sequencizer/interrupter.h"
#include "modules/sequencizer/trigger-mode.h"

#include "dheunit/test.h"
#include "fixtures/interrupter-fixture.h"

#include <sstream>
#include <vector>

namespace test {
namespace sequencizer {
using dhe::Latch;
using dhe::sequencizer::InterruptMode;
using dhe::sequencizer::TriggerMode;
using dhe::unit::Suite;
using dhe::unit::Tester;

static auto constexpr low_latch = dhe::latch::low;
static auto constexpr high_latch = dhe::latch::high;
static auto constexpr falling_latch = dhe::latch::falling;
static auto constexpr rising_latch = dhe::latch::rising;

struct InterrupterTestCase {
  InterruptMode interrupt_mode_; // NOLINT
  TriggerMode trigger_mode_;     // NOLINT
  Latch latch_;                  // NOLINT
  bool is_interrupted_;          // NOLINT

  void run(Tester &t) const {
    t.run(name(), [this](Tester &t) {
      auto constexpr step = 3;
      Signals signals{};
      Interrupter interrupter{signals};
      signals.trigger_mode_[step] = trigger_mode_;
      signals.interrupt_mode_[step] = interrupt_mode_;

      bool got = interrupter.is_interrupted(step, latch_);
      if (got != is_interrupted_) {
        t.errorf("Got {}, want {}", got, is_interrupted_);
      }
    });
  }

  auto name() const -> std::string {
    auto os = std::ostringstream{};
    os << std::boolalpha << interrupt_mode_ << " + " << trigger_mode_ << " + "
       << latch_ << " → " << is_interrupted_;
    return os.str();
  }
};

auto test_cases = std::vector<InterrupterTestCase>{
    {InterruptMode::No, TriggerMode::GateRises, rising_latch, false},
    {InterruptMode::No, TriggerMode::GateRises, falling_latch, false},
    {InterruptMode::No, TriggerMode::GateRises, high_latch, false},
    {InterruptMode::No, TriggerMode::GateRises, low_latch, false},
    {InterruptMode::No, TriggerMode::GateFalls, rising_latch, false},
    {InterruptMode::No, TriggerMode::GateFalls, falling_latch, false},
    {InterruptMode::No, TriggerMode::GateFalls, high_latch, false},
    {InterruptMode::No, TriggerMode::GateFalls, low_latch, false},
    {InterruptMode::No, TriggerMode::GateChanges, rising_latch, false},
    {InterruptMode::No, TriggerMode::GateChanges, falling_latch, false},
    {InterruptMode::No, TriggerMode::GateChanges, high_latch, false},
    {InterruptMode::No, TriggerMode::GateChanges, low_latch, false},
    {InterruptMode::No, TriggerMode::GateIsHigh, rising_latch, false},
    {InterruptMode::No, TriggerMode::GateIsHigh, falling_latch, false},
    {InterruptMode::No, TriggerMode::GateIsHigh, high_latch, false},
    {InterruptMode::No, TriggerMode::GateIsHigh, low_latch, false},
    {InterruptMode::No, TriggerMode::GateIsLow, rising_latch, false},
    {InterruptMode::No, TriggerMode::GateIsLow, falling_latch, false},
    {InterruptMode::No, TriggerMode::GateIsLow, high_latch, false},
    {InterruptMode::No, TriggerMode::GateIsLow, low_latch, false},
    {InterruptMode::Yes, TriggerMode::GateRises, rising_latch, true},
    {InterruptMode::Yes, TriggerMode::GateRises, falling_latch, false},
    {InterruptMode::Yes, TriggerMode::GateRises, high_latch, false},
    {InterruptMode::Yes, TriggerMode::GateRises, low_latch, false},
    {InterruptMode::Yes, TriggerMode::GateFalls, rising_latch, false},
    {InterruptMode::Yes, TriggerMode::GateFalls, falling_latch, true},
    {InterruptMode::Yes, TriggerMode::GateFalls, high_latch, false},
    {InterruptMode::Yes, TriggerMode::GateFalls, low_latch, false},
    {InterruptMode::Yes, TriggerMode::GateChanges, rising_latch, true},
    {InterruptMode::Yes, TriggerMode::GateChanges, falling_latch, true},
    {InterruptMode::Yes, TriggerMode::GateChanges, high_latch, false},
    {InterruptMode::Yes, TriggerMode::GateChanges, low_latch, false},
    {InterruptMode::Yes, TriggerMode::GateIsHigh, rising_latch, true},
    {InterruptMode::Yes, TriggerMode::GateIsHigh, falling_latch, false},
    {InterruptMode::Yes, TriggerMode::GateIsHigh, high_latch, true},
    {InterruptMode::Yes, TriggerMode::GateIsHigh, low_latch, false},
    {InterruptMode::Yes, TriggerMode::GateIsLow, rising_latch, false},
    {InterruptMode::Yes, TriggerMode::GateIsLow, falling_latch, true},
    {InterruptMode::Yes, TriggerMode::GateIsLow, high_latch, false},
    {InterruptMode::Yes, TriggerMode::GateIsLow, low_latch, true},
};

class InterrupterSuite : public Suite {
public:
  InterrupterSuite()
      : Suite{"dhe::sequencizer::Interrupter is_interrupted(s)"} {}
  void run(Tester &t) override {
    for (auto const &test : test_cases) {
      test.run(t);
    }
  }
};

static auto _ = InterrupterSuite{};
} // namespace sequencizer
} // namespace test
