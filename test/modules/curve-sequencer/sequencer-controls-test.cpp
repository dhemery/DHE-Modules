#include "controls-test.h"
#include "dheunit/assertions.h"

namespace test {
namespace curve_sequencer {
namespace sequence_controls {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class SequenceControlsTests : Suite {
public:
  SequenceControlsTests() : Suite{"dhe::curve_sequencer::[Sequence]Controls"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("is_gated()", test([](Tester &t, Module &module, Controls &controls) {
          module.set(Controls::Param::Gate, 1.F);
          module.set(Controls::Input::Gate, 0.F);

          t.assert_that("button pressed", controls.is_gated(), is_true);

          module.set(Controls::Param::Gate, 0.F);
          module.set(Controls::Input::Gate, 10.F);
          t.assert_that("input high", controls.is_gated(), is_true);

          module.set(Controls::Param::Gate, 0.F);
          module.set(Controls::Input::Gate, 0.F);
          t.assert_that("input low, button not pressed", controls.is_gated(),
                        is_false);
        }));

    add("is_looping()", test([](Tester &t, Module &module, Controls &controls) {
          module.set(Controls::Param::Loop, 1.F);
          module.set(Controls::Input::Loop, 0.F);
          t.assert_that("button pressed", controls.is_looping(), is_true);

          module.set(Controls::Param::Loop, 0.F);
          module.set(Controls::Input::Loop, 10.F);
          t.assert_that("input high", controls.is_looping(), is_true);

          module.set(Controls::Param::Loop, 0.F);
          module.set(Controls::Input::Loop, 0.F);
          t.assert_that("input low, button not pressed", controls.is_looping(),
                        is_false);
        }));

    add("is_reset()", test([](Tester &t, Module &module, Controls &controls) {
          module.set(Controls::Param::Reset, 1.F);
          module.set(Controls::Input::Reset, 0.F);
          t.assert_that("button pressed", controls.is_reset(), is_true);

          module.set(Controls::Param::Reset, 0.F);
          module.set(Controls::Input::Reset, 10.F);
          t.assert_that("input high", controls.is_reset(), is_true);

          module.set(Controls::Param::Reset, 0.F);
          module.set(Controls::Input::Reset, 0.F);
          t.assert_that("input low, button not pressed", controls.is_reset(),
                        is_false);
        }));

    add("is_running()", test([](Tester &t, Module &module, Controls &controls) {
          module.set(Controls::Param::Run, 1.F);
          module.set(Controls::Input::Run, 0.F);
          t.assert_that("button pressed", controls.is_running(), is_true);

          module.set(Controls::Param::Run, 0.F);
          module.set(Controls::Input::Run, 10.F);
          t.assert_that("input high", controls.is_running(), is_true);

          module.set(Controls::Param::Run, 0.F);
          module.set(Controls::Input::Run, 0.F);
          t.assert_that("input low, button not pressed", controls.is_running(),
                        is_false);
        }));

    add("selection_start()",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr start = 2;
          module.set(Controls::Param::SelectionStart, start);
          t.assert_that(controls.selection_start(), is_equal_to(start));
        }));

    add("selection_length()",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr length = 5;
          module.set(Controls::Param::SelectionLength, length);
          t.assert_that(controls.selection_length(), is_equal_to(length));
        }));

    add("input()", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr input = 7.777F;
          module.set(Controls::Input::CurveSequencer, input);
          t.assert_that(controls.input(), is_equal_to(input));
        }));

    add("output()", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr output = 3.333F;
          module.set(Controls::Output::CurveSequencer, output);
          t.assert_that(controls.output(), is_equal_to(output));
        }));

    add("output(v)", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr output = 4.444F;
          controls.output(output);
          t.assert_that(module.get(Controls::Output::CurveSequencer),
                        is_equal_to(output));
        }));
  }
};

__attribute__((unused)) static SequenceControlsTests sequence_controls_tests{};

} // namespace sequence_controls
} // namespace curve_sequencer
} // namespace test
