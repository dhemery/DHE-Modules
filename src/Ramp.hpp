#pragma once
#include <functional>

namespace DHE {

// A linear ramp that progresses from 0 to 1.
// When the ramp reaches 1, it remains at 1
// until reset or restarted.
struct Ramp {
    std::function<void()> onEoC;
    bool running = false;
    float value = 0.0;

    // Constructs a ramp that calls the given function
    // on every End of Cycle.
    Ramp(std::function<void()> onEoC);

    // Starts the ramp with a value of 0.
    void start();

    // Stops the ramp with a value of 0.
    void stop();

    // Advances the ramp value by an amount that,
    // at the engine's current sample rate, would
    // transition from 0 to 1 over the given duration
    // (in seconds). Calls the EoC function this advances the ramp from
    // less than 1 to 1, the onEoC function is called.
    void step(std::function<float()> deltaSupplier);
};
} // namespace DHE
