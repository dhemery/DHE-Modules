#include "modules/swiss-army-sequencer/anchor.h"
#include <array>
#include <dheunit/test.h>

namespace test {
namespace swiss_army_sequencer {

static auto constexpr step_count = 8;
using dhe::swiss_army_sequencer::AnchorMode;
using dhe::swiss_army_sequencer::AnchorSource;
using dhe::swiss_army_sequencer::AnchorType;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

struct Module {
  auto anchor_level(AnchorType type, int step) const -> float {
    return type == AnchorType::Start ? start_level_[step] : end_level_[step];
  }
  auto anchor_mode(AnchorType type, int step) const -> AnchorMode {
    return type == AnchorType::Start ? start_mode_[step] : end_mode_[step];
  }
  auto anchor_source(AnchorType type, int step) const -> AnchorSource {
    return type == AnchorType::Start ? start_source_[step] : end_source_[step];
  }
  auto aux() const -> float { return aux_; }
  auto input() const -> float { return input_; }
  auto output() const -> float { return output_; }

  float aux_{};                                         // NOLINT
  float input_{};                                       // NOLINT
  float output_{};                                      // NOLINT
  std::array<float, step_count> end_level_{};           // NOLINT
  std::array<AnchorMode, step_count> end_mode_{};       // NOLINT
  std::array<AnchorSource, step_count> end_source_{};   // NOLINT
  std::array<float, step_count> start_level_{};         // NOLINT
  std::array<AnchorMode, step_count> start_mode_{};     // NOLINT
  std::array<AnchorSource, step_count> start_source_{}; // NOLINT
};

using Anchor = dhe::swiss_army_sequencer::Anchor<Module>;

template <typename Run>
static inline auto test(AnchorType type, Run const &run) -> TestFunc {
  return [type, run](Tester &t) {
    Module module{};
    Anchor anchor{module, type};
    run(t, module, anchor);
  };
}

} // namespace swiss_army_sequencer
} // namespace test
