#include "./fixtures/generator-fixture.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace scannibal {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::TestRegistrar;

class GeneratorSuite : public Suite {
public:
  GeneratorSuite() : Suite{"dhe::scannibal::Generator::generate()"} {}
  void register_tests(TestRegistrar add) override {
    add("outputs scaled tapered phase",
        test([](Tester &t, Module &module, Anchor &start_anchor,
                Anchor &end_anchor, Generator &generator) {
          auto constexpr step = 4;

          auto constexpr phase = 0.5F; // halfway
          auto constexpr taper = dhe::sigmoid::j_taper;
          auto constexpr curvature = 0.5F; // linear

          auto constexpr start_voltage = 0.F;
          auto constexpr end_voltage = 7.F;
          auto constexpr scaled_tapered_phase =
              (end_voltage - start_voltage) * taper.apply(phase, curvature);

          start_anchor.voltage_ = start_voltage;
          end_anchor.voltage_ = end_voltage;

          module.taper_[step] = &taper;
          module.curvature_[step] = curvature;

          generator.generate(step, phase);

          t.assert_that(module.output_, is_equal_to(scaled_tapered_phase));
        }));

    add("first call: "
        "enters anchors at executed step",
        test([](Tester &t, Module &module, Anchor &start_anchor,
                Anchor &end_anchor, Generator &generator) {
          auto constexpr step = 3;
          module.taper_[step] = &dhe::sigmoid::j_taper;
          ;

          generator.generate(step, 0.F);

          t.assert_that(start_anchor.entered_[step], is_true);
          t.assert_that(end_anchor.entered_[step], is_true);
        }));

    add("if called with same step: "
        "does not enter anchors",
        test([](Tester &t, Module &module, Anchor &start_anchor,
                Anchor &end_anchor, Generator &generator) {
          auto constexpr step = 6;
          module.taper_[step] = &dhe::sigmoid::j_taper;

          generator.generate(step, 0.F);

          start_anchor.entered_[step] = false;
          end_anchor.entered_[step] = false;

          generator.generate(step, 0.11F);
          generator.generate(step, 0.62F);

          t.assert_that(start_anchor.entered_[step], is_false);
          t.assert_that(end_anchor.entered_[step], is_false);
        }));

    add("if called with changed step: "
        "enters anchors at new step",
        test([](Tester &t, Module &module, Anchor &start_anchor,
                Anchor &end_anchor, Generator &generator) {
          auto constexpr first_step = 6;
          auto constexpr second_step = 2;
          module.taper_[first_step] = &dhe::sigmoid::j_taper;
          module.taper_[second_step] = &dhe::sigmoid::j_taper;

          generator.generate(first_step, 0.11F);

          generator.generate(second_step, 0.11F);

          t.assert_that(start_anchor.entered_[second_step], is_true);
          t.assert_that(end_anchor.entered_[second_step], is_true);
        }));
  }
};

__attribute__((unused)) static auto _ = GeneratorSuite{};
} // namespace scannibal
} // namespace test
