#pragma once

#include "modules/components/Latch.h"

namespace dhe {
namespace curve_sequencer {
  static auto constexpr generateModeCount = 7;
  static auto constexpr generateDefaultMode = 6;
  static auto constexpr sustainModeCount = 6;
  static auto constexpr sustainDefaultMode = 5;

  template <typename C> class GenerateStage {
  public:
    auto execute(int, Latch const &, float) -> bool { return false; }
  };

  template <typename C> class SustainStage {
  public:
    auto execute(int, Latch const &) -> bool { return false; }
  };
} // namespace curve_sequencer
} // namespace dhe
