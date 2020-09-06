#include "./fixtures/controls-fixture.h"
#include "dheunit/assertions.h"

namespace test {
namespace curve_sequencer {
using dhe::unit::is_equal_to;
using dhe::unit::is_false;
using dhe::unit::is_true;
using dhe::unit::Suite;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

class SequenceControlsSuite : Suite {
public:
  SequenceControlsSuite() : Suite{"dhe::curve_sequencer::Controls/sequence"} {}

  void register_tests(dhe::unit::TestRegistrar add) override {
    add("gate()", test([](Tester &t, Module &module, Controls &controls) {
          module.set_param(Param::Gate, 1.F);
          module.set_input(Input::Gate, 0.F);

          t.assert_that("button pressed", controls.gate(), is_true);

          module.set_param(Param::Gate, 0.F);
          module.set_input(Input::Gate, 10.F);
          t.assert_that("input high", controls.gate(), is_true);

          module.set_param(Param::Gate, 0.F);
          module.set_input(Input::Gate, 0.F);
          t.assert_that("input low, button not pressed", controls.gate(),
                        is_false);
        }));

    add("is_looping()", test([](Tester &t, Module &module, Controls &controls) {
          module.set_param(Param::Loop, 1.F);
          module.set_input(Input::Loop, 0.F);
          t.assert_that("button pressed", controls.is_looping(), is_true);

          module.set_param(Param::Loop, 0.F);
          module.set_input(Input::Loop, 10.F);
          t.assert_that("input high", controls.is_looping(), is_true);

          module.set_param(Param::Loop, 0.F);
          module.set_input(Input::Loop, 0.F);
          t.assert_that("input low, button not pressed", controls.is_looping(),
                        is_false);
        }));

    add("is_reset()", test([](Tester &t, Module &module, Controls &controls) {
          module.set_param(Param::Reset, 1.F);
          module.set_input(Input::Reset, 0.F);
          t.assert_that("button pressed", controls.is_reset(), is_true);

          module.set_param(Param::Reset, 0.F);
          module.set_input(Input::Reset, 10.F);
          t.assert_that("input high", controls.is_reset(), is_true);

          module.set_param(Param::Reset, 0.F);
          module.set_input(Input::Reset, 0.F);
          t.assert_that("input low, button not pressed", controls.is_reset(),
                        is_false);
        }));

    add("is_running()", test([](Tester &t, Module &module, Controls &controls) {
          module.set_param(Param::Run, 1.F);
          module.set_input(Input::Run, 0.F);
          t.assert_that("button pressed", controls.is_running(), is_true);

          module.set_param(Param::Run, 0.F);
          module.set_input(Input::Run, 10.F);
          t.assert_that("input high", controls.is_running(), is_true);

          module.set_param(Param::Run, 0.F);
          module.set_input(Input::Run, 0.F);
          t.assert_that("input low, button not pressed", controls.is_running(),
                        is_false);
        }));

    add("selection_start()",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr start = 2;
          module.set_param(Param::SelectionStart, start);
          t.assert_that(controls.selection_start(), is_equal_to(start));
        }));

    add("selection_length()",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr length = 5;
          module.set_param(Param::SelectionLength, length);
          t.assert_that(controls.selection_length(), is_equal_to(length));
        }));

    add("input()", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr input = 7.777F;
          module.set_input(Input::In, input);
          t.assert_that(controls.input(), is_equal_to(input));
        }));

    add("output(v)", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr output = 4.444F;
          controls.output(output);
          t.assert_that(module.get_output(Output::Out), is_equal_to(output));
        }));
  }
};

__attribute__((unused)) static auto _ = SequenceControlsSuite{};

} // namespace curve_sequencer
} // namespace test
