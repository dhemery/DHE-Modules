#include "./fixtures/generator-fixture.h"

#include <dheunit/assertions.h>
#include <dheunit/test.h>

namespace test {
namespace scannibal {
using dhe::unit::is_equal_to;
using dhe::unit::Suite;
using dhe::unit::TestRegistrar;

class GeneratorSuite : public Suite {
public:
  GeneratorSuite() : Suite{"dhe::scannibal::Generator"} {}
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
  }
};

__attribute__((unused)) static auto _ = GeneratorSuite{};
} // namespace scannibal
} // namespace test
