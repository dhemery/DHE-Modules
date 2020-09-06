#include "modules/curve-sequencer/generator.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

using dhe::unit::Tester;
using dhe::unit::TestFunc;

struct Anchor {
  void enter(int step) { entered_step_ = step; }

  int entered_step_{}; // NOLINT
};

struct Module {
  void show_progress(int step, float progress) {
    progress_step_ = step;
    progress_ = progress;
  }
  void show_inactive(int step) { inactive_step_ = step; }
  float progress_{};    // NOLINT
  int progress_step_{}; // NOLINT
  int inactive_step_{}; // NOLINT
};

using Generator = dhe::curve_sequencer::Generator<Module, Anchor>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Module module{};
    Anchor start_anchor{};
    Anchor end_anchor{};
    Generator generator{module, start_anchor, end_anchor};
    run(t, module, start_anchor, end_anchor, generator);
  };
}

} // namespace curve_sequencer
} // namespace test
