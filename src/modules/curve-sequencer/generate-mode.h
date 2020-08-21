#pragma once

namespace dhe {
namespace curve_sequencer {
enum class GenerateMode { Curve, Hold, Sustain, Input, Chase, Level };

static auto constexpr generate_mode_count =
    static_cast<int>(GenerateMode::Level) + 1;
} // namespace curve_sequencer
} // namespace dhe
