#pragma once

#include "modules/sequencizer/anchor.h"

#include <dheunit/test.h>

#include <array>
#include <functional>

namespace test {
namespace sequencizer {

static auto constexpr step_count = 8;
using dhe::sequencizer::AnchorMode;
using dhe::sequencizer::AnchorSource;
using dhe::sequencizer::AnchorType;
using dhe::unit::Tester;

using TestFunc = std::function<void(Tester &)>;

struct Signals {
  auto anchor_level(AnchorType type, int step) const -> float {
    return type == AnchorType::Start ? start_level_[step] : end_level_[step];
  }
  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    return type == AnchorType::Start ? start_mode_[step] : end_mode_[step];
  }
  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    return type == AnchorType::Start ? start_source_[step] : end_source_[step];
  }
  auto in_a() const -> float { return in_a_; }
  auto in_b() const -> float { return in_b_; }
  auto in_c() const -> float { return in_c_; }
  auto output() const -> float { return output_; }

  float in_a_{};                                        // NOLINT
  float in_b_{};                                        // NOLINT
  float in_c_{};                                        // NOLINT
  float output_{};                                      // NOLINT
  std::array<AnchorMode, step_count> end_mode_{};       // NOLINT
  std::array<float, step_count> end_level_{};           // NOLINT
  std::array<AnchorSource, step_count> end_source_{};   // NOLINT
  std::array<AnchorMode, step_count> start_mode_{};     // NOLINT
  std::array<float, step_count> start_level_{};         // NOLINT
  std::array<AnchorSource, step_count> start_source_{}; // NOLINT
};

using Anchor = dhe::sequencizer::Anchor<Signals>;

template <typename Run>
static inline auto test(AnchorType type, Run const &run) -> TestFunc {
  return [type, run](Tester &t) {
    Signals signals{};
    Anchor anchor{signals, type};
    run(t, signals, anchor);
  };
}

} // namespace sequencizer
} // namespace test
