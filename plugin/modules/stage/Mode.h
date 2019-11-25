#pragma once

namespace dhe {
namespace stage {
  enum class Mode {
    Defer,      // Defer to upstream module (or IN port) by sending the IN voltage to the OUT port
    Generate,   // Generate a curve
    Hold,       // Hold the current voltage for the duration
    Idle,       // Do nothing
    Sustain,    // Sustain the current voltage until GATE falls
    TrackInput, // Send the IN voltage to the OUT port
    TrackLevel, // Send the LEVEL voltage to the OUT port
  };

} // namespace stage
} // namespace dhe