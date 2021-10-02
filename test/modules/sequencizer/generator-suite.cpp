#include "./fixtures/generator-fixture.h"
#include "./fixtures/status-enums.h"

#include "modules/sequencizer/status.h"

#include "helpers/assertions.h"
#include <dheunit/test.h>

namespace test {
namespace sequencizer {
using dhe::sequencizer::GeneratorStatus;
using dhe::unit::Suite;
using test::is_equal_to;
using test::is_true;

class GeneratorSuite : public Suite {
public:
  GeneratorSuite() : Suite{"dhe::sequencizer::Generator"} {}
  void run(Tester &t) override {
    t.run("start(s) enters anchors at step",
          test([](Tester &t, Module & /*module*/, Anchor &start_anchor,
                  Anchor &end_source, Generator &generator) {
            auto constexpr step = 1;

            generator.start(step);

            assert_that(t, "start anchor", start_anchor.entered_[step],
                        is_true);
            assert_that(t, "end anchor", end_source.entered_[step], is_true);
          }));

    t.run("start(s) shows progress at 0",
          test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                  Anchor & /*end_source*/, Generator &generator) {
            auto constexpr step = 7;
            module.taper_[step] = &dhe::sigmoid::j_taper;
            module.duration_[step] = 10.F;

            generator.start(step);

            assert_that(t, "first start", module.progress_[step],
                        is_equal_to(0.F));

            generator.generate(3.F); // Advance a bit

            generator.start(step);
            assert_that(t, "start after progress", module.progress_[step],
                        is_equal_to(0.F));
          }));

    t.run("stop()",
          test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                  Anchor & /*end_anchor*/, Generator &generator) {
            auto constexpr step = 2;

            generator.start(step);
            generator.stop();

            assert_that(t, "shows step inactive", module.inactive_step_,
                        is_equal_to(step));
          }));

    t.run("generate(t) outputs_ scaled tapered phase",
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

            module.duration_[step] = duration;
            start_anchor.voltage_ = start_voltage;
            end_anchor.voltage_ = end_voltage;

            module.taper_[step] = &taper;
            module.curvature_[step] = curvature;

            generator.generate(sample_time);

            assert_that(t, module.output_, is_equal_to(scaled_tapered_phase));
          }));

    t.run("generate(t) reports progress",
          test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                  Anchor & /*end_anchor*/, Generator &generator) {
            auto constexpr step = 3;

            module.taper_[step] = &dhe::sigmoid::j_taper;
            generator.start(step);

            auto constexpr duration = 10.F;   // 10 sec
            auto constexpr sample_time = 3.F; // 3 sec
            auto constexpr phase_delta = sample_time / duration;
            module.duration_[step] = duration;
            module.progress_[step] = -1.F;

            generator.generate(sample_time);

            assert_that(t, "after 1st generate", module.progress_[step],
                        is_equal_to(phase_delta));
            module.progress_[step] = -1.F;

            generator.generate(sample_time);

            assert_that(t, "after 2nd generate", module.progress_[step],
                        is_equal_to(phase_delta + phase_delta));
          }));

    t.run("generate(t) reports whether duration has expired",
          test([](Tester &t, Module &module, Anchor & /*start_anchor*/,
                  Anchor & /*end_anchor*/, Generator &generator) {
            auto constexpr step = 3;
            module.taper_[step] = &dhe::sigmoid::j_taper;

            generator.start(step);

            auto constexpr duration = 10.F;
            module.duration_[step] = duration;

            // Big enough to complete the duration on the second generate
            auto constexpr sample_time = duration * 0.6F;

            auto status = generator.generate(sample_time);

            assert_that(t, "not expired", status,
                        is_equal_to(GeneratorStatus::Generating));

            status = generator.generate(sample_time);

            assert_that(t, "expires", status,
                        is_equal_to(GeneratorStatus::Completed));

            status = generator.generate(sample_time);

            assert_that(t, "previously expired", status,
                        is_equal_to(GeneratorStatus::Completed));
          }));
  }
};

static auto _ = GeneratorSuite{};
} // namespace sequencizer
} // namespace test
