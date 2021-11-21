#pragma once

#include "modules/curve-sequencer/generate-mode.h"

#include <array>
#include <ostream>
#include <string>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::GenerateMode;
using dhe::curve_sequencer::GenerateModes;

static auto constexpr generate_modes =
    std::array<GenerateMode, GenerateModes::size>{
        GenerateMode::Curve, GenerateMode::Hold,  GenerateMode::Sustain,
        GenerateMode::Input, GenerateMode::Chase, GenerateMode::Level,
    };

static inline auto name_of(GenerateMode mode) -> std::string {
  switch (mode) {
  case GenerateMode::Curve:
    return "GenerateMode::Curve";
  case GenerateMode::Hold:
    return "GenerateMode::Hold";
  case GenerateMode::Sustain:
    return "GenerateMode::Sustain";
  case GenerateMode::Input:
    return "GenerateMode::InPort";
  case GenerateMode::Chase:
    return "GenerateMode::Chase";
  case GenerateMode::Level:
    return "GenerateMode::Level";
  default:
    return "Unknown GenerateMode " + std::to_string(static_cast<int>(mode));
  }
}
} // namespace curve_sequencer
} // namespace test

namespace dhe {
namespace curve_sequencer {

static inline auto operator<<(std::ostream &os, GenerateMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}
} // namespace curve_sequencer
} // namespace dhe
