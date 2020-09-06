#include "./fixtures/advancement-enums.h"
#include "./fixtures/interrupter-fixture.h"
#include <dheunit/assertions.h>
#include <helpers/latches.h>
#include <sstream>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::Suite;

struct InterrupterTestCase {
  InterrupterTestCase(InterruptMode interrupt_mode, TriggerMode trigger_mode,
                      Latch latch, bool is_interrupted)
      : interrupt_mode_{interrupt_mode}, trigger_mode_{trigger_mode},
        latch_{latch}, is_interrupted_{is_interrupted} {}
  InterruptMode interrupt_mode_{}; // NOLINT
  TriggerMode trigger_mode_{};     // NOLINT
  Latch latch_{};                  // NOLINT
  bool is_interrupted_{};          // NOLINT

  void operator()(Tester &t) const {
    auto constexpr step = 3;
    Controls controls{};
    Interrupter interrupter{controls};
    controls.trigger_mode_[step] = trigger_mode_;
    controls.interrupt_mode_[step] = interrupt_mode_;

    t.assert_that(interrupter.is_interrupted(step, latch_),
                  is_equal_to(is_interrupted_));
  }

  auto name() const -> std::string {
    auto os = std::ostringstream{};
    os << std::boolalpha << interrupt_mode_ << " + " << trigger_mode_ << " + "
       << latch_ << " → " << is_interrupted_;
    return os.str();
  }
};

class InterrupterSuite : public Suite {
public:
  InterrupterSuite()
      : Suite{"dhe::curve_sequencer::Interrupter is_interrupted(s)"} {}
  void register_tests(dhe::unit::TestRegistrar add) override {
    auto test_cases = std::vector<InterrupterTestCase>{
        {InterruptMode::Advance, TriggerMode::GateRises, rising_latch, true},
        {InterruptMode::Advance, TriggerMode::GateRises, falling_latch, false},
        {InterruptMode::Advance, TriggerMode::GateRises, high_latch, false},
        {InterruptMode::Advance, TriggerMode::GateRises, low_latch, false},
        {InterruptMode::Advance, TriggerMode::GateFalls, rising_latch, false},
        {InterruptMode::Advance, TriggerMode::GateFalls, falling_latch, true},
        {InterruptMode::Advance, TriggerMode::GateFalls, high_latch, false},
        {InterruptMode::Advance, TriggerMode::GateFalls, low_latch, false},
        {InterruptMode::Advance, TriggerMode::GateChanges, rising_latch, true},
        {InterruptMode::Advance, TriggerMode::GateChanges, falling_latch, true},
        {InterruptMode::Advance, TriggerMode::GateChanges, high_latch, false},
        {InterruptMode::Advance, TriggerMode::GateChanges, low_latch, false},
        {InterruptMode::Advance, TriggerMode::GateIsHigh, rising_latch, true},
        {InterruptMode::Advance, TriggerMode::GateIsHigh, falling_latch, false},
        {InterruptMode::Advance, TriggerMode::GateIsHigh, high_latch, true},
        {InterruptMode::Advance, TriggerMode::GateIsHigh, low_latch, false},
        {InterruptMode::Advance, TriggerMode::GateIsLow, rising_latch, false},
        {InterruptMode::Advance, TriggerMode::GateIsLow, falling_latch, true},
        {InterruptMode::Advance, TriggerMode::GateIsLow, high_latch, false},
        {InterruptMode::Advance, TriggerMode::GateIsLow, low_latch, true},
        {InterruptMode::Ignore, TriggerMode::GateRises, rising_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateRises, falling_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateRises, high_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateRises, low_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateFalls, rising_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateFalls, falling_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateFalls, high_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateFalls, low_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateChanges, rising_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateChanges, falling_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateChanges, high_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateChanges, low_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateIsHigh, rising_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateIsHigh, falling_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateIsHigh, high_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateIsHigh, low_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateIsLow, rising_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateIsLow, falling_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateIsLow, high_latch, false},
        {InterruptMode::Ignore, TriggerMode::GateIsLow, low_latch, false},
    };

    for (auto const &test_case : test_cases) {
      add(test_case.name(), test_case);
    }
  }
};

__attribute__((unused)) static auto _ = InterrupterSuite{};
} // namespace curve_sequencer
} // namespace test
