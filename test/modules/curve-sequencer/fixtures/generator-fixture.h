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

struct Controls {
  void show_progress(int step, float progress) {
    activated_step_ = step;
    progress_ = progress;
  }
  void show_inactive(int step) { deactivated_step_ = step; }
  float progress_{};       // NOLINT
  int activated_step_{};   // NOLINT
  int deactivated_step_{}; // NOLINT
};

using Generator = dhe::curve_sequencer::Generator<Controls, Anchor>;

template <typename GeneratorTest>
static inline auto test(GeneratorTest const &generator_test) -> TestFunc {
  return [generator_test](Tester &t) {
    Controls controls{};
    Anchor start_anchor{};
    Anchor end_anchor{};
    Generator generator{controls, start_anchor, end_anchor};
    generator_test(t, controls, start_anchor, end_anchor, generator);
  };
}

} // namespace curve_sequencer
} // namespace test
