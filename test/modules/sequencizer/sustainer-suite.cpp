#include "./fixtures/advancement-enums.h"
#include "./fixtures/sustainer-fixture.h"
#include "helpers/latches.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace sequencizer {
using dhe::unit::is_equal_to;
using dhe::unit::Suite;
using dhe::unit::Tester;

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

    t.assert_that(sustainer.is_done(step, latch_), is_equal_to(is_done_));
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
  void register_tests(dhe::unit::TestRegistrar add) override {
    auto test_cases = std::vector<SustainerTestCase>{
        {SustainMode::Sustain, TriggerMode::GateRises, rising_latch, true},
        {SustainMode::Sustain, TriggerMode::GateRises, falling_latch, false},
        {SustainMode::Sustain, TriggerMode::GateRises, high_latch, false},
        {SustainMode::Sustain, TriggerMode::GateRises, low_latch, false},
        {SustainMode::Sustain, TriggerMode::GateFalls, rising_latch, false},
        {SustainMode::Sustain, TriggerMode::GateFalls, falling_latch, true},
        {SustainMode::Sustain, TriggerMode::GateFalls, high_latch, false},
        {SustainMode::Sustain, TriggerMode::GateFalls, low_latch, false},
        {SustainMode::Sustain, TriggerMode::GateChanges, rising_latch, true},
        {SustainMode::Sustain, TriggerMode::GateChanges, falling_latch, true},
        {SustainMode::Sustain, TriggerMode::GateChanges, high_latch, false},
        {SustainMode::Sustain, TriggerMode::GateChanges, low_latch, false},
        {SustainMode::Sustain, TriggerMode::GateIsHigh, rising_latch, true},
        {SustainMode::Sustain, TriggerMode::GateIsHigh, falling_latch, false},
        {SustainMode::Sustain, TriggerMode::GateIsHigh, high_latch, true},
        {SustainMode::Sustain, TriggerMode::GateIsHigh, low_latch, false},
        {SustainMode::Sustain, TriggerMode::GateIsLow, rising_latch, false},
        {SustainMode::Sustain, TriggerMode::GateIsLow, falling_latch, true},
        {SustainMode::Sustain, TriggerMode::GateIsLow, high_latch, false},
        {SustainMode::Sustain, TriggerMode::GateIsLow, low_latch, true},
        {SustainMode::Advance, TriggerMode::GateRises, rising_latch, true},
        {SustainMode::Advance, TriggerMode::GateRises, falling_latch, true},
        {SustainMode::Advance, TriggerMode::GateRises, high_latch, true},
        {SustainMode::Advance, TriggerMode::GateRises, low_latch, true},
        {SustainMode::Advance, TriggerMode::GateFalls, rising_latch, true},
        {SustainMode::Advance, TriggerMode::GateFalls, falling_latch, true},
        {SustainMode::Advance, TriggerMode::GateFalls, high_latch, true},
        {SustainMode::Advance, TriggerMode::GateFalls, low_latch, true},
        {SustainMode::Advance, TriggerMode::GateChanges, rising_latch, true},
        {SustainMode::Advance, TriggerMode::GateChanges, falling_latch, true},
        {SustainMode::Advance, TriggerMode::GateChanges, high_latch, true},
        {SustainMode::Advance, TriggerMode::GateChanges, low_latch, true},
        {SustainMode::Advance, TriggerMode::GateIsHigh, rising_latch, true},
        {SustainMode::Advance, TriggerMode::GateIsHigh, falling_latch, true},
        {SustainMode::Advance, TriggerMode::GateIsHigh, high_latch, true},
        {SustainMode::Advance, TriggerMode::GateIsHigh, low_latch, true},
        {SustainMode::Advance, TriggerMode::GateIsLow, rising_latch, true},
        {SustainMode::Advance, TriggerMode::GateIsLow, falling_latch, true},
        {SustainMode::Advance, TriggerMode::GateIsLow, high_latch, true},
        {SustainMode::Advance, TriggerMode::GateIsLow, low_latch, true},
    };
    for (auto const &test_case : test_cases) {
      add(test_case.name(), test_case);
    }
  }
};

__attribute__((unused)) static auto _ = SustainerSuite{};
} // namespace sequencizer
} // namespace test
