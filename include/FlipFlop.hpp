#pragma once

#include <functional>
#include <memory>

namespace DHE {
    struct FlipFlop {
        enum State {
            UNKNOWN, LOW, HIGH
        };

        // Creates a new flip-flop that evaluates the given signal on each step
        // and reacts to changes in state.
        FlipFlop(const std::function<float()> &signal,
                 float lowThreshold, float highThreshold,
                 const std::function<void()> &onRise = [](){},
                 const std::function<void()> &onFall = [](){},
                 const std::function<void()> &onNoChange = [](){});

        FlipFlop(const std::function<float()> &signal,
                 const std::function<void()> &onRise = [](){},
                 const std::function<void()> &onFall = [](){},
                 const std::function<void()> &onNoChange = [](){})
        : FlipFlop(signal, 0.0f, 1.0f, onRise, onFall, onNoChange){}

        // Evaluates the signal and sets the state accordingly.
        // Fires onRise() if the state changed to HIGH.
        // Fires onFall() if the state changed to LOW.
        // Fires onNoChange() if the state did not change.
        void step();

        bool isHigh();

        bool isLow();

    private:
        float lowThreshold;
        float highThreshold;
        State _state = UNKNOWN;
        std::function<float()> signal;
        std::function<void()> onRise;
        std::function<void()> onFall;
        std::function<void()> onNoChange;
    };
} // namespace DHE
