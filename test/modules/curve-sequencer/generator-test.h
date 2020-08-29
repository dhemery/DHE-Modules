#include "modules/curve-sequencer/generator.h"
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

using dhe::unit::Tester;
using dhe::unit::TestFunc;

struct Controls {
  void show_progress(int step, float progress) {
    activated_step_ = step;
    progress_ = progress;
  }
  void show_inactive(int step) { deactivated_step_ = step; }
  float progress_{-999.F};   // NOLINT
  int activated_step_{-3};   // NOLINT
  int deactivated_step_{-3}; // NOLINT
};

struct Source {
  void snap(int step) { snapped_step_ = step; }

  int snapped_step_{-3}; // NOLINT
};

using Generator = dhe::curve_sequencer::Generator<Controls, Source>;

template <typename GeneratorTest>
static inline auto test(GeneratorTest const &generator_test) -> TestFunc {
  return [generator_test](Tester &t) {
    Controls controls{};
    Source start{};
    Source end{};
    Generator generator{controls, start, end};
    generator_test(t, controls, start, end, generator);
  };
}

} // namespace curve_sequencer
} // namespace test
