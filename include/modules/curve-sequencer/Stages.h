#pragma once

#include "modules/components/Latch.h"

#include <array>
#include <string>

namespace dhe {
namespace curve_sequencer {
  static auto constexpr generateModeCount = 7;
  static auto constexpr generateDefaultMode = 6;
  static auto constexpr sustainModeCount = 6;
  static auto constexpr sustainDefaultMode = 5;

  static auto const generateModeDescriptions = std::array<std::string, generateModeCount>{
      "Stop curve if gate rises",   "Stop curve if gate falls",  "Stop curve if gate changes",
      "Stop curve if gate is high", "Stop curve if gate is low", "No curve",
      "Generate entire curve"};

  static auto const sustainModeDescriptions = std::array<std::string, sustainModeCount>{
      "Sustain until gate rises",   "Sustain until gate falls",  "Sustain until gate changes",
      "Sustain until gate is high", "Sustain until gate is low", "No sustain"};

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
