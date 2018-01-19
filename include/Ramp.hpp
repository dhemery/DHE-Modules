#pragma once

#include <functional>

namespace DHE {

// A linear ramp that progresses from 0 to 1. When the ramp reaches 1, it
// remains at 1 until stopped or restarted.
    struct Ramp {
        // Constructs a ramp that advances in amounts supplied by delta(), and that
        // calls eoc() at the end of each step that advances the ramp to 1.
        Ramp(const std::function<float()> &delta, const std::function<void()> &eoc);

        // Starts the ramp with a value of 0.
        void start();

        // Stops the ramp with a value of 0.
        void stop();

        // Advances the ramp by the amount specified by the delta function up to a
        // maximum value of 1. If the step advances the ramp to 1, the ramp stops
        // running with a value of 1 and calls eoc(). If the ramp is not running,
        // this function has no effect.
        void step();

        bool isRunning();

        float value();

    private:
        std::function<void()> eoc;
        std::function<float()> delta;
        bool _running;
        float _value;
    };
} // namespace DHE
