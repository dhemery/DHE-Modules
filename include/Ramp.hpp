#pragma once

#include <functional>

namespace DHE {

// A linear ramp that progresses from 0 to 1. When the ramp reaches 1, it
// remains at 1 until stopped or restarted.
    struct Ramp {
        // Constructs a ramp that advances in steps supplied by the step supplier,
        // and that calls onEndOfCycle() at the end of each step that advances
        // the ramp to 1.
        Ramp(const std::function<float()> &step, const std::function<void()> &onEndOfCycle);

        // Starts the ramp with a value of 0.
        void start();

        // Stops the ramp with a value of 0.
        void stop();

        // Advances the ramp by the amount specified by the step supplier up to a
        // maximum value of 1. If the step advances the ramp to 1, the ramp stops
        // running with a value of 1 and calls onEndOfCycle(). If the ramp is not
        // running, this function has no effect.
        void step();

        bool isRunning();

        float value();

    private:
        std::function<void()> cycleEnded;
        std::function<float()> delta;
        bool running;
        float progress;
    };
} // namespace DHE
