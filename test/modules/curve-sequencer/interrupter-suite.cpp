#include "./fixtures/interrupt-modes.h"
#include "./fixtures/interrupter-fixture.h"
#include "./fixtures/trigger-modes.h"
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
    };

    for (auto const &test_case : test_cases) {
      add(test_case.name(), test_case);
    }
  }
};

__attribute__((unused)) static auto _ = InterrupterSuite{};
} // namespace curve_sequencer
} // namespace test
