#include "./fixtures/generator-fixture.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace scannibal {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;

class GeneratorSuite : public Suite {
public:
  GeneratorSuite() : Suite{"dhe::scannibal::Generator::generate()"} {}
  void run(Tester &t) override {
    t.run("outputs scaled tapered phase",
          test([](Tester &t, Module &module, Anchor &phase_0_anchor,
                  Anchor &phase_1_anchor, Generator &generator) {
            auto constexpr step = 4;

            auto constexpr phase = 0.5F; // halfway
            auto constexpr taper = dhe::sigmoid::j_taper;
            auto constexpr curvature = 0.5F; // linear

            auto constexpr phase_0_voltage = 0.F;
            auto constexpr phase_1_voltage = 7.F;
            auto constexpr scaled_tapered_phase =
                (phase_1_voltage - phase_0_voltage) *
                taper.apply(phase, curvature);

            phase_0_anchor.voltage_ = phase_0_voltage;
            phase_1_anchor.voltage_ = phase_1_voltage;

            module.taper_[step] = &taper;
            module.curvature_[step] = curvature;

            generator.generate(step, phase);

            t.assert_that(module.output_, is_equal_to(scaled_tapered_phase));
          }));

    t.run("first call: "
          "enters anchors at executed step",
          test([](Tester &t, Module &module, Anchor &phase_0_anchor,
                  Anchor &phase_1_anchor, Generator &generator) {
            auto constexpr step = 3;
            module.taper_[step] = &dhe::sigmoid::j_taper;
            ;

            generator.generate(step, 0.F);

            t.assert_that(phase_0_anchor.entered_[step], is_true);
            t.assert_that(phase_1_anchor.entered_[step], is_true);
          }));

    t.run("if called with same step: "
          "does not enter anchors",
          test([](Tester &t, Module &module, Anchor &phase_0_anchor,
                  Anchor &phase_1_anchor, Generator &generator) {
            auto constexpr step = 6;
            module.taper_[step] = &dhe::sigmoid::j_taper;

            generator.generate(step, 0.F);

            phase_0_anchor.entered_[step] = false;
            phase_1_anchor.entered_[step] = false;

            generator.generate(step, 0.11F);
            generator.generate(step, 0.62F);

            t.assert_that(phase_0_anchor.entered_[step], is_false);
            t.assert_that(phase_1_anchor.entered_[step], is_false);
          }));

    t.run("if called with changed step: "
          "enters anchors at new step",
          test([](Tester &t, Module &module, Anchor &phase_0_anchor,
                  Anchor &phase_1_anchor, Generator &generator) {
            auto constexpr first_step = 6;
            auto constexpr second_step = 2;
            module.taper_[first_step] = &dhe::sigmoid::j_taper;
            module.taper_[second_step] = &dhe::sigmoid::j_taper;

            generator.generate(first_step, 0.11F);

            generator.generate(second_step, 0.11F);

            t.assert_that(phase_0_anchor.entered_[second_step], is_true);
            t.assert_that(phase_1_anchor.entered_[second_step], is_true);
          }));
  }
};

__attribute__((unused)) static auto _ = GeneratorSuite{};
} // namespace scannibal
} // namespace test
