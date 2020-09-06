#include "./fixtures/generator-fixture.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::Suite;
using dhe::unit::TestRegistrar;

class GeneratorSuite : public Suite {
public:
  GeneratorSuite() : Suite{"dhe::curve_sequencer::Generator"} {}
  void register_tests(TestRegistrar add) override {
    add("start(s)", test([](Tester &t, Module &module, Anchor &start_anchor,
                            Anchor &end_source, Generator &generator) {
          auto constexpr started_step = 3;

          generator.start(started_step);

          t.assert_that("shows progress for step", module.progress_step_,
                        is_equal_to(started_step));
          t.assert_that("shows progress", module.progress_, is_equal_to(0.F));
          t.assert_that("enters start anchor at step",
                        start_anchor.entered_step_, is_equal_to(started_step));
          t.assert_that("enters end anchor at step", end_source.entered_step_,
                        is_equal_to(started_step));
        }));

    add("stop()", test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                          Anchor & /*end_anchor*/, Generator &generator) {
          auto constexpr step = 5;

          generator.start(step);
          generator.stop();

          t.assert_that("shows step inactive", module.inactive_step_,
                        is_equal_to(step));
        }));

    // TODO: generate
    //  computes the phase delta from the sample time and duration
    //  advances the phase timer by the phase delta
    //  tapers the phase by applying the taper from the module
    //  computes output voltage from tapered phase, start voltage, and end
    //  voltage reports progress emits the output voltage returns true if the
    //  phase timer is not in progress returns false if the phase timer is in
    //  progress
  }
};

__attribute__((unused)) static auto _ = GeneratorSuite{};
} // namespace curve_sequencer
} // namespace test
