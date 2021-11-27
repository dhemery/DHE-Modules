#include "fixtures/generator-fixture.h"

#include "helpers/assertions.h"

#include "dheunit/test.h"

namespace test {
namespace scannibal {
using dhe::Shape;
using dhe::unit::Suite;
using test::is_equal_to;
using test::is_false;
using test::is_true;

class GeneratorSuite : public Suite {
public:
  GeneratorSuite() : Suite{"dhe::scannibal::Generator::generate()"} {}
  void run(Tester &t) override {
    t.run("outputs_ scaled tapered phase",
          test([](Tester &t, Module &module, Anchor &phase_0_anchor,
                  Anchor &phase_1_anchor, Generator &generator) {
            auto constexpr step = 4;

            auto constexpr phase = 0.5F; // halfway
            auto constexpr shape = Shape::Id::J;
            auto constexpr curvature = 0.5F; // linear

            auto constexpr phase_0_voltage = 0.F;
            auto constexpr phase_1_voltage = 7.F;
            auto const scaled_tapered_phase =
                (phase_1_voltage - phase_0_voltage) *
                Shape::apply(phase, shape, curvature);

            phase_0_anchor.voltage_ = phase_0_voltage;
            phase_1_anchor.voltage_ = phase_1_voltage;

            module.shape_[step] = shape;
            module.curvature_[step] = curvature;

            generator.generate(step, phase);

            assert_that(t, module.output_, is_equal_to(scaled_tapered_phase));
          }));

    t.run("first call: "
          "enters anchors at executed step",
          test([](Tester &t, Module &module, Anchor &phase_0_anchor,
                  Anchor &phase_1_anchor, Generator &generator) {
            auto constexpr step = 3;
            module.shape_[step] = Shape::Id::J;

            generator.generate(step, 0.F);

            assert_that(t, phase_0_anchor.entered_[step], is_true);
            assert_that(t, phase_1_anchor.entered_[step], is_true);
          }));

    t.run("if called with same step: "
          "does not enter anchors",
          test([](Tester &t, Module &module, Anchor &phase_0_anchor,
                  Anchor &phase_1_anchor, Generator &generator) {
            auto constexpr step = 6;
            module.shape_[step] = Shape::Id::J;

            generator.generate(step, 0.F);

            phase_0_anchor.entered_[step] = false;
            phase_1_anchor.entered_[step] = false;

            generator.generate(step, 0.11F);
            generator.generate(step, 0.62F);

            assert_that(t, phase_0_anchor.entered_[step], is_false);
            assert_that(t, phase_1_anchor.entered_[step], is_false);
          }));

    t.run("if called with changed step: "
          "enters anchors at new step",
          test([](Tester &t, Module &module, Anchor &phase_0_anchor,
                  Anchor &phase_1_anchor, Generator &generator) {
            auto constexpr first_step = 6;
            auto constexpr second_step = 2;
            module.shape_[first_step] = Shape::Id::J;
            module.shape_[second_step] = Shape::Id::J;

            generator.generate(first_step, 0.11F);

            generator.generate(second_step, 0.11F);

            assert_that(t, phase_0_anchor.entered_[second_step], is_true);
            assert_that(t, phase_1_anchor.entered_[second_step], is_true);
          }));
  }
};

static auto _ = GeneratorSuite{};
} // namespace scannibal
} // namespace test
