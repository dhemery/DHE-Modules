#include "./fixtures/advancement-enums.h"
#include "./fixtures/sustainer-fixture.h"
#include "helpers/assertions.h"
#include "helpers/latches.h"

#include <dheunit/test.h>
#include <vector>

namespace test {
namespace sequencizer {
using dhe::unit::Suite;
using dhe::unit::Tester;
using test::is_equal_to;

struct SustainerTestCase {
  SustainerTestCase(SustainMode completion_mode, TriggerMode trigger_mode,
                    Latch latch, bool is_done)
      : completion_mode_{completion_mode},
        trigger_mode_{trigger_mode}, latch_{latch}, is_done_{is_done} {}
  SustainMode completion_mode_{}; // NOLINT
  TriggerMode trigger_mode_{};    // NOLINT
  Latch latch_{};                 // NOLINT
  bool is_done_{};                // NOLINT

  void operator()(Tester &t) const {
    auto constexpr step = 3;
    Module module{};
    Sustainer sustainer{module};
    module.trigger_mode_[step] = trigger_mode_;
    module.completion_mode_[step] = completion_mode_;

    assert_that(t, sustainer.is_done(step, latch_), is_equal_to(is_done_));
  }

  auto name() const -> std::string {
    auto os = std::ostringstream{};
    os << std::boolalpha << completion_mode_ << " + " << trigger_mode_ << " + "
       << latch_ << " → " << is_done_;
    return os.str();
  }
};

class SustainerSuite : public Suite {
public:
  SustainerSuite() : Suite{"dhe::sequencizer::Sustainer is_done(s)"} {}
  void run(Tester &t) override {
    auto test_cases = std::vector<SustainerTestCase>{
        {SustainMode::No, TriggerMode::GateRises, rising_latch, true},
        {SustainMode::No, TriggerMode::GateRises, falling_latch, true},
        {SustainMode::No, TriggerMode::GateRises, high_latch, true},
        {SustainMode::No, TriggerMode::GateRises, low_latch, true},
        {SustainMode::No, TriggerMode::GateFalls, rising_latch, true},
        {SustainMode::No, TriggerMode::GateFalls, falling_latch, true},
        {SustainMode::No, TriggerMode::GateFalls, high_latch, true},
        {SustainMode::No, TriggerMode::GateFalls, low_latch, true},
        {SustainMode::No, TriggerMode::GateChanges, rising_latch, true},
        {SustainMode::No, TriggerMode::GateChanges, falling_latch, true},
        {SustainMode::No, TriggerMode::GateChanges, high_latch, true},
        {SustainMode::No, TriggerMode::GateChanges, low_latch, true},
        {SustainMode::No, TriggerMode::GateIsHigh, rising_latch, true},
        {SustainMode::No, TriggerMode::GateIsHigh, falling_latch, true},
        {SustainMode::No, TriggerMode::GateIsHigh, high_latch, true},
        {SustainMode::No, TriggerMode::GateIsHigh, low_latch, true},
        {SustainMode::No, TriggerMode::GateIsLow, rising_latch, true},
        {SustainMode::No, TriggerMode::GateIsLow, falling_latch, true},
        {SustainMode::No, TriggerMode::GateIsLow, high_latch, true},
        {SustainMode::No, TriggerMode::GateIsLow, low_latch, true},
        {SustainMode::Yes, TriggerMode::GateRises, rising_latch, true},
        {SustainMode::Yes, TriggerMode::GateRises, falling_latch, false},
        {SustainMode::Yes, TriggerMode::GateRises, high_latch, false},
        {SustainMode::Yes, TriggerMode::GateRises, low_latch, false},
        {SustainMode::Yes, TriggerMode::GateFalls, rising_latch, false},
        {SustainMode::Yes, TriggerMode::GateFalls, falling_latch, true},
        {SustainMode::Yes, TriggerMode::GateFalls, high_latch, false},
        {SustainMode::Yes, TriggerMode::GateFalls, low_latch, false},
        {SustainMode::Yes, TriggerMode::GateChanges, rising_latch, true},
        {SustainMode::Yes, TriggerMode::GateChanges, falling_latch, true},
        {SustainMode::Yes, TriggerMode::GateChanges, high_latch, false},
        {SustainMode::Yes, TriggerMode::GateChanges, low_latch, false},
        {SustainMode::Yes, TriggerMode::GateIsHigh, rising_latch, true},
        {SustainMode::Yes, TriggerMode::GateIsHigh, falling_latch, false},
        {SustainMode::Yes, TriggerMode::GateIsHigh, high_latch, true},
        {SustainMode::Yes, TriggerMode::GateIsHigh, low_latch, false},
        {SustainMode::Yes, TriggerMode::GateIsLow, rising_latch, false},
        {SustainMode::Yes, TriggerMode::GateIsLow, falling_latch, true},
        {SustainMode::Yes, TriggerMode::GateIsLow, high_latch, false},
        {SustainMode::Yes, TriggerMode::GateIsLow, low_latch, true},
    };
    for (auto const &test_case : test_cases) {
      t.run(test_case.name(), test_case);
    }
  }
};

__attribute__((unused)) static auto _ = SustainerSuite{};
} // namespace sequencizer
} // namespace test
