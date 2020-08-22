#pragma once

namespace dhe {
namespace stage {
enum class Mode {
  Defer, // Defer to upstream module (or IN port) by sending the IN voltage to
         // the OUT port
  Generate, // Generate a curve over the duration
  Hold,     // Hold the current voltage for the duration
  Idle,     // Do nothing
  Input,    // Send the IN voltage to the OUT port
  Level,    // Send the LEVEL voltage to the OUT port
  Sustain,  // Sustain the current voltage until GATE falls
};

} // namespace stage
} // namespace dhe