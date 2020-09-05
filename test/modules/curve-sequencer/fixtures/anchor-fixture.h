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
  auto end_level(int step) const -> float { return end_level_[step]; }
  auto end_anchor_mode(int step) const -> AnchorMode {
    return end_anchor_mode_[step];
  }
  auto end_anchor_source(int step) const -> AnchorSource {
    return end_anchor_source_[step];
  }
  auto input() const -> float { return input_; }
  auto output() const -> float { return output_; }
  auto start_level(int step) const -> float { return start_level_[step]; }
  auto start_anchor_mode(int step) const -> AnchorMode {
    return start_anchor_mode_[step];
  }
  auto start_anchor_source(int step) const -> AnchorSource {
    return start_anchor_source_[step];
  }
  std::array<AnchorMode, step_count> end_anchor_mode_{};       // NOLINT
  std::array<AnchorSource, step_count> end_anchor_source_{};   // NOLINT
  std::array<float, step_count> end_level_{};                  // NOLINT
  float input_{};                                              // NOLINT
  float output_{};                                             // NOLINT
  std::array<AnchorMode, step_count> start_anchor_mode_{};     // NOLINT
  std::array<AnchorSource, step_count> start_anchor_source_{}; // NOLINT
  std::array<float, step_count> start_level_{};                // NOLINT
};

using EndAnchor = dhe::curve_sequencer::EndAnchor<Controls>;

using StartAnchor = dhe::curve_sequencer::StartAnchor<Controls>;

} // namespace curve_sequencer
} // namespace test
