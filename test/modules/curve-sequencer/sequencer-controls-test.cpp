#include "controls-test.h"

namespace test {
namespace curve_sequencer {
namespace sequence_controls {
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
          if (!controls.is_gated()) {
            t.error("button pressed: got false, want true");
          }

          module.set(Controls::Param::Gate, 0.F);
          module.set(Controls::Input::Gate, 10.F);
          if (!controls.is_gated()) {
            t.error("input high: got false, want true");
          }

          module.set(Controls::Param::Gate, 0.F);
          module.set(Controls::Input::Gate, 0.F);
          if (controls.is_gated()) {
            t.error("input low and button released: got false, want true");
          }
        }));

    add("is_looping()", test([](Tester &t, Module &module, Controls &controls) {
          module.set(Controls::Param::Loop, 1.F);
          module.set(Controls::Input::Loop, 0.F);
          if (!controls.is_looping()) {
            t.error("button pressed: got false, want true");
          }

          module.set(Controls::Param::Loop, 0.F);
          module.set(Controls::Input::Loop, 10.F);
          if (!controls.is_looping()) {
            t.error("input high: got false, want true");
          }

          module.set(Controls::Param::Loop, 0.F);
          module.set(Controls::Input::Loop, 0.F);
          if (controls.is_looping()) {
            t.error("input low and button not pressed: got false, want true");
          }
        }));

    add("is_reset()", test([](Tester &t, Module &module, Controls &controls) {
          module.set(Controls::Param::Reset, 1.F);
          module.set(Controls::Input::Reset, 0.F);
          if (!controls.is_reset()) {
            t.error("button pressed: got false, want true");
          }

          module.set(Controls::Param::Reset, 0.F);
          module.set(Controls::Input::Reset, 10.F);
          if (!controls.is_reset()) {
            t.error("input high: got false, want true");
          }

          module.set(Controls::Param::Reset, 0.F);
          module.set(Controls::Input::Reset, 0.F);
          if (controls.is_reset()) {
            t.error("input low and button not pressed: got false, want true");
          }
        }));

    add("is_running()", test([](Tester &t, Module &module, Controls &controls) {
          module.set(Controls::Param::Run, 1.F);
          module.set(Controls::Input::Run, 0.F);
          if (!controls.is_running()) {
            t.error("button pressed: got false, want true");
          }

          module.set(Controls::Param::Run, 0.F);
          module.set(Controls::Input::Run, 10.F);
          if (!controls.is_running()) {
            t.error("input high: got false, want true");
          }

          module.set(Controls::Param::Run, 0.F);
          module.set(Controls::Input::Run, 0.F);
          if (controls.is_running()) {
            t.error("input low and button not pressed: got true, want false");
          }
        }));

    add("selection_start()",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr start = 2;
          module.set(Controls::Param::SelectionStart, start);

          int got = controls.selection_start();
          if (got != start) {
            t.error("got {}, want {}", got, start);
          }
        }));

    add("selection_length()",
        test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr length = 5;
          module.set(Controls::Param::SelectionLength, length);

          int got = controls.selection_length();
          if (got != length) {
            t.error("got {}, want {}", got, length);
          }
        }));

    add("input()", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr input = 7.777F;
          module.set(Controls::Input::CurveSequencer, input);

          float got = controls.input();
          if (got != input) {
            t.error("got {}, want {}", got, input);
          }
        }));

    add("output()", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr output = 3.333F;
          module.set(Controls::Output::CurveSequencer, output);

          float got = controls.output();
          if (got != output) {
            t.error("got {}, want {}", got, output);
          }
        }));

    add("output(v)", test([](Tester &t, Module &module, Controls &controls) {
          auto constexpr output = 4.444F;

          controls.output(output);

          auto const got = module.get(Controls::Output::CurveSequencer);
          if (got != output) {
            t.error("got {}, want {}", got, output);
          }
        }));
  }
};

__attribute__((unused)) static SequenceControlsTests sequence_controls_tests{};

} // namespace sequence_controls
} // namespace curve_sequencer
} // namespace test
