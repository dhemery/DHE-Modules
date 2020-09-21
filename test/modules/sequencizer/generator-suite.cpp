#include "./fixtures/generator-fixture.h"
#include "./fixtures/status-enums.h"

#include "modules/sequencizer/status.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace sequencizer {
using dhe::sequencizer::GeneratorStatus;
using dhe::unit::is_equal_to;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::TestRegistrar;

class GeneratorSuite : public Suite {
public:
  GeneratorSuite() : Suite{"dhe::sequencizer::Generator"} {}
  void register_tests(TestRegistrar add) override {
    add("start(s) enters anchors at step",
        test([](Tester &t, Module & /*module*/, Anchor &start_anchor,
                Anchor &end_source, Generator &generator) {
          auto constexpr step = 1;

          generator.start(step);

          t.assert_that("start anchor", start_anchor.entered_[step], is_true);
          t.assert_that("end anchor", end_source.entered_[step], is_true);
        }));

    add("start(s) shows progress at 0",
        test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                Anchor & /*end_source*/, Generator &generator) {
          auto constexpr step = 7;
          module.taper_[step] = &dhe::sigmoid::j_taper;
          module.duration_ = 10.F;

          generator.start(step);

          t.assert_that("first start", module.progress_[step],
                        is_equal_to(0.F));

          generator.generate(3.F); // Advance a bit

          generator.start(step);
          t.assert_that("start after progress", module.progress_[step],
                        is_equal_to(0.F));
        }));

    add("stop()", test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                          Anchor & /*end_anchor*/, Generator &generator) {
          auto constexpr step = 2;

          generator.start(step);
          generator.stop();

          t.assert_that("shows step inactive", module.inactive_step_,
                        is_equal_to(step));
        }));

    add("generate(t) outputs scaled tapered phase",
        test([](Tester &t, Module &module, Anchor &start_anchor,
                Anchor &end_anchor, Generator &generator) {
          auto constexpr step = 4;
          generator.start(step);

          auto constexpr duration = 10.F; // 10 sec

          auto constexpr phase = 0.5F;
          auto constexpr sample_time = duration * phase;
          auto constexpr taper = dhe::sigmoid::j_taper;
          auto constexpr curvature = 0.5F;

          auto constexpr start_voltage = 0.F;
          auto constexpr end_voltage = 7.F;
          auto constexpr scaled_tapered_phase =
              (end_voltage - start_voltage) * taper.apply(phase, curvature);

          module.duration_ = duration;
          start_anchor.voltage_ = start_voltage;
          end_anchor.voltage_ = end_voltage;

          module.taper_[step] = &taper;
          module.curvature_[step] = curvature;

          generator.generate(sample_time);

          t.assert_that(module.output_, is_equal_to(scaled_tapered_phase));
        }));

    add("generate(t) reports progress",
        test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                Anchor & /*end_anchor*/, Generator &generator) {
          auto constexpr step = 3;

          module.taper_[step] = &dhe::sigmoid::j_taper;
          generator.start(step);

          auto constexpr duration = 10.F;   // 10 sec
          auto constexpr sample_time = 3.F; // 3 sec
          auto constexpr phase_delta = sample_time / duration;
          module.duration_ = duration;
          module.progress_[step] = -1.F;

          generator.generate(sample_time);

          t.assert_that("after 1st generate", module.progress_[step],
                        is_equal_to(phase_delta));
          module.progress_[step] = -1.F;

          generator.generate(sample_time);

          t.assert_that("after 2nd generate", module.progress_[step],
                        is_equal_to(phase_delta + phase_delta));
        }));

    add("generate(t) reports whether duration has expired",
        test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                Anchor & /*end_anchor*/, Generator &generator) {
          auto constexpr step = 3;
          module.taper_[step] = &dhe::sigmoid::j_taper;

          generator.start(step);

          auto constexpr duration = 10.F;
          module.duration_ = duration;

          // Big enough to complete the duration on the second generate
          auto constexpr sample_time = duration * 0.6F;

          auto status = generator.generate(sample_time);

          t.assert_that("not expired", status,
                        is_equal_to(GeneratorStatus::Generating));

          status = generator.generate(sample_time);

          t.assert_that("expires", status,
                        is_equal_to(GeneratorStatus::Completed));

          status = generator.generate(sample_time);

          t.assert_that("previously expired", status,
                        is_equal_to(GeneratorStatus::Completed));
        }));
  }
};

__attribute__((unused)) static auto _ = GeneratorSuite{};
} // namespace sequencizer
} // namespace test
