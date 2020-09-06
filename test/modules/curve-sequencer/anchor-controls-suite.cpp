#include "./fixtures/anchor-controls-fixture.h"
#include "./fixtures/anchor-modes.h"
#include "./fixtures/anchor-sources.h"
#include "controls/level-inputs.h"
#include <dheunit/assertions.h>
#include <dheunit/test.h>
#include <string>

namespace test {
namespace curve_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::Suite;

class AnchorControlsSuite : Suite {
public:
  AnchorControlsSuite() : Suite{"dhe::curve_sequencer::AnchorControls"} {}
  void register_tests(dhe::unit::TestRegistrar add_test) override {

    for (auto const &mode : anchor_modes) {
      add_test(std::string{"mode(s): "} + name_of(mode),
               test([mode](Tester &t, Module &module, Controls &controls) {
                 auto constexpr step = 1;

                 module.set_param(Param::StepStartAnchorMode, step,
                                  static_cast<float>(mode));

                 t.assert_that(controls.mode(step), is_equal_to(mode));
               }));
    }

    for (auto const &source : anchor_sources) {
      add_test(std::string{"source(s): "} + name_of(source),
               test([source](Tester &t, Module &module, Controls &controls) {
                 auto constexpr step = 3;

                 module.set_param(Param::StepStartAnchorSource, step,
                                  static_cast<float>(source));

                 t.assert_that(controls.source(step), is_equal_to(source));
               }));
    }

    add_test(
        "level(s)", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr step = 7;
          auto constexpr rotation = 0.37F;
          auto constexpr range = 1; // unipolar 0–10
          module.set_param(Param::StepStartLevel, step, rotation);
          module.set_param(Param::LevelRange, range);

          t.assert_that(
              controls.level(step),
              is_equal_to(dhe::level(rotation, dhe::signal_ranges[range])));
        }));

    add_test("input()", test([](Tester &t, Module &module, Controls &controls) {
               auto constexpr input = 43.2349F;
               module.set_input(Input::In, input);
               t.assert_that(controls.input(), is_equal_to(input));
             }));

    add_test("output()",
             test([](Tester &t, Module &module, Controls &controls) {
               auto constexpr output = 0.123F;
               module.set_output(Output::Out, output);
               t.assert_that(controls.output(), is_equal_to(output));
             }));
  }
};

__attribute__((unused)) static auto _ = AnchorControlsSuite{};
} // namespace curve_sequencer
} // namespace test
