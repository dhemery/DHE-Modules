#include "modules/scannibal/anchor.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace scannibal {

static auto constexpr step_count = 8;
using dhe::scannibal::AnchorMode;
using dhe::scannibal::AnchorSource;
using dhe::scannibal::AnchorType;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

struct Module {
  auto anchor_level(AnchorType type, int step) const -> float {
    return type == AnchorType::Phase0 ? phase_0_level_[step]
                                      : phase_1_level_[step];
  }
  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    return type == AnchorType::Phase0 ? phase_0_mode_[step]
                                      : phase_1_mode_[step];
  }
  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    return type == AnchorType::Phase0 ? phase_0_source_[step]
                                      : phase_1_source_[step];
  }
  auto in_a() const -> float { return in_a_; }
  auto in_b() const -> float { return in_b_; }
  auto in_c() const -> float { return in_c_; }
  auto output() const -> float { return output_; }

  float in_a_{};                                          // NOLINT
  float in_b_{};                                          // NOLINT
  float in_c_{};                                          // NOLINT
  float output_{};                                        // NOLINT
  std::array<float, step_count> phase_0_level_{};         // NOLINT
  std::array<AnchorMode, step_count> phase_0_mode_{};     // NOLINT
  std::array<AnchorSource, step_count> phase_0_source_{}; // NOLINT
  std::array<float, step_count> phase_1_level_{};         // NOLINT
  std::array<AnchorMode, step_count> phase_1_mode_{};     // NOLINT
  std::array<AnchorSource, step_count> phase_1_source_{}; // NOLINT
};

using Anchor = dhe::scannibal::Anchor<Module>;

template <typename Run>
static inline auto test(AnchorType type, Run const &run) -> TestFunc {
  return [type, run](Tester &t) {
    Module module{};
    Anchor anchor{module, type};
    run(t, module, anchor);
  };
}

} // namespace scannibal
} // namespace test
