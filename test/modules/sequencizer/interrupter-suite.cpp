#include "./fixtures/advancement-enums.h"
#include "./fixtures/interrupter-fixture.h"
#include <dheunit/assertions.h>
#include <helpers/latches.h>
#include <sstream>
#include <vector>

namespace test {
namespace sequencizer {
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
    Module module{};
    Interrupter interrupter{module};
    module.trigger_mode_[step] = trigger_mode_;
    module.interrupt_mode_[step] = interrupt_mode_;

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
      : Suite{"dhe::sequencizer::Interrupter is_interrupted(s)"} {}
  void run(Tester &t) override {
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

    for (auto const &test_case : test_cases) {
      t.run(test_case.name(), test_case);
    }
  }
};

__attribute__((unused)) static auto _ = InterrupterSuite{};
} // namespace sequencizer
} // namespace test
