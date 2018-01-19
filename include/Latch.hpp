#pragma once

#include <functional>

namespace DHE {
    struct Latch {
        enum State {
            UNKNOWN, LOW, HIGH
        };

        // Constructs a latch with the given thresholds
        // and an initial state of UNKNOWN.
        Latch(float low = 0.0f, float high = 0.1f);

        // Resets the latch to UNKNOWN state.
        void reset();

        // Updates the latch and calls the function if the latch
        // rose from UNKNOWN or LOW to HIGH.
        void process(float value, const std::function<void()> &onRise = [](){}, const std::function<void()> &onFall = [](){}, const std::function<void()> &onNoChange= [](){});

        bool isHigh();

        bool isLow();

        bool isUnknownState();

        State state();

    private:
        float _low;
        float _high;
        State _state = UNKNOWN;
    };
} // namespace DHE
