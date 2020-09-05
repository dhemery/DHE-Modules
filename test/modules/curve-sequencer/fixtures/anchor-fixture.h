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
  auto output() const -> float { return output_; }
  auto start_level(int step) const -> float { return start_level_[step]; }
  auto start_anchor_mode(int step) const -> AnchorMode {
    return start_anchor_mode_[step];
  }
  auto start_anchor_source(int step) const -> AnchorSource {
    return start_anchor_source_[step];
  }
  float input_{};                                              // NOLINT
  float output_{};                                             // NOLINT
  std::array<AnchorMode, step_count> start_anchor_mode_{};     // NOLINT
  std::array<AnchorSource, step_count> start_anchor_source_{}; // NOLINT
  std::array<float, step_count> start_level_{};                // NOLINT
};

using StartAnchor = dhe::curve_sequencer::StartAnchor<Controls>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Controls controls{};
    StartAnchor anchor{controls};
    run(t, controls, anchor);
  };
}

} // namespace curve_sequencer
} // namespace test
