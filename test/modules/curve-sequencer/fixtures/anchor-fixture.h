#include "modules/curve-sequencer/anchor-mode.h"
#include "modules/curve-sequencer/anchor-source.h"
#include "modules/curve-sequencer/anchor.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace curve_sequencer {

static auto constexpr step_count = 8;
using dhe::curve_sequencer::AnchorMode;
using dhe::curve_sequencer::AnchorSource;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

struct Controls {
  auto input() const -> float { return input_; }
  auto level(int step) const -> float { return level_[step]; }
  auto mode(int step) const -> AnchorMode { return mode_[step]; }
  auto output() const -> float { return output_; }
  auto source(int step) const -> AnchorSource { return source_[step]; }

  std::array<AnchorMode, step_count> mode_{};     // NOLINT
  std::array<AnchorSource, step_count> source_{}; // NOLINT
  float input_{};                                 // NOLINT
  std::array<float, step_count> level_{};         // NOLINT
  float output_{};                                // NOLINT
};

using Anchor = dhe::curve_sequencer::Anchor<Controls>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Controls controls{};
    Anchor anchor{controls};
    run(t, controls, anchor);
  };
}

} // namespace curve_sequencer
} // namespace test
