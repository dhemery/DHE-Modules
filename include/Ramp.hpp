#pragma once

#include <functional>

namespace DHE {

    // A ramp whose phase progresses from 0 to 1. When the phase reaches 1,
    // further positive increments have no effect.
    struct Ramp {
        // Constructs a ramp that advances in increments supplied by the supplier,
        // and that calls onEndOfCycle() at the end of each step that advances
        // the phase to 1.
        Ramp(const std::function<float()> &phaseIncrementSupplier, const std::function<void()> &onEndOfCycle);

        // Starts the ramp at phase 0.
        void start();

        // Stops the ramp at phase 0.
        void stop();

        // Advances the phase by the supplied increment, to a maximum phase of 1.
        // If the phase advances to 1, the ramp stops running with phase==1 and
        // calls onEndOfCycle(). If the ramp is not running, this function has no
        // effect.
        void step();

        bool isRunning();

        float phase();

    private:
        std::function<void()> _onEndOfCycle;
        std::function<float()> _phaseIncrement;
        bool _running;
        float _phase;
    };
} // namespace DHE
